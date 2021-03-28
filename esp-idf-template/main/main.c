#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_timer.h"

#include "driver/mcpwm.h"
#include "soc/mcpwm_reg.h"
#include "soc/mcpwm_struct.h"

#define RIGHT_TACH1 18
#define RIGHT_TACH2 19

#define LEFT_TACH1 34
#define LEFT_TACH2 35
#define LEFT_TACH_SAMPLE_INTERVAL 1000 // microseconds

#define ESP_INTR_FLAG_DEFAULT 0


#define GPIO_INPUT_PIN_SEL  ((1ULL<<RIGHT_TACH1) | (1ULL<<RIGHT_TACH2) | (1ULL<<LEFT_TACH1) | (1ULL<<LEFT_TACH2))

#define RIGHT_PWM 26   //Set GPIO 15 as PWM0A
#define RIGHT_MOTOR_ENABLE 27   //Set GPIO 16 as PWM0B

#define LEFT_PWM 25   //Set GPIO 15 as PWM0A
#define LEFT_MOTOR_ENABLE 33   //Set GPIO 16 as PWM0B



float right_position = 0 ;
float right_position_previous  = 0 ;
static float left_position = 0 ;
static float left_position_previous = 0 ;

static float right_velocity;
static float left_velocity;

typedef enum { Stopped = 0, Forwards = 1, Backwards =2, Invalid = 3 } Quad_out;

typedef struct fsmState {
    Quad_out out[4];
    int next[4];
} FSM_state ;


#define TACH_CNT_PER_CYCLE 12.0
#define GEAR_RATIO 30.0
#define PI 3.1415
#define WHEEL_DIAMETER_MM 9000.0 // cm
#define wheel_circumference  PI *  WHEEL_DIAMETER_MM
#define STEP_SIZE_mm  WHEEL_DIAMETER_MM /   (TACH_CNT_PER_CYCLE * GEAR_RATIO ) 

static xQueueHandle right_encoder_evt_queue = NULL;
static xQueueHandle left_encoder_evt_queue = NULL;

static xQueueHandle velocity_evt_queue = NULL;


FSM_state FSM[4] = {
    {{Stopped, Backwards, Forwards, Invalid}, {0, 1, 2, 0}}, // 00
    {{Forwards, Stopped, Invalid, Backwards}, {0, 1, 1, 3}}, // 01
    {{Backwards, Invalid, Stopped, Forwards}, {0, 2, 2, 3}}, // 10
    {{Invalid, Forwards, Backwards, Stopped}, {3, 1, 2, 3}}  // 11
};
int right_fsm_current_state = 0;
int left_fsm_current_state = 0;

float getPositionChange(int a, int b, int* FSM_current_state )
{
    int input = (a * 2) + b;
    int output = FSM[*FSM_current_state].out[input];
    float returnValue = 0;

    *FSM_current_state = FSM[*FSM_current_state].next[input];
    switch (output)    {
        case Stopped:
            break;
        case Forwards:
            returnValue =  1;
            // returnValue =  STEP_SIZE_mm;
            break;
        case Backwards:
            returnValue =   -1;
            // returnValue =   -1* STEP_SIZE_mm;
            break;
        default:
            // motorState.invalidCount++;
            break;
    }
    return returnValue;
}
static void IRAM_ATTR right_encoder_isr_handler(void* arg)
{
    int a = gpio_get_level(RIGHT_TACH1);
    int b = gpio_get_level(RIGHT_TACH2);

    float  returnValue = getPositionChange(a, b, &right_fsm_current_state) ;

    xQueueSendFromISR(right_encoder_evt_queue, &returnValue, NULL);


}


static void IRAM_ATTR left_encoder_isr_handler(void* arg)
{
    int a = gpio_get_level(LEFT_TACH1);
    int b = gpio_get_level(LEFT_TACH2);
    float  returnValue = getPositionChange(a, b, &left_fsm_current_state);


    xQueueSendFromISR(left_encoder_evt_queue, &returnValue, NULL);

}


void encoder_initialize(){
    gpio_config_t io_conf;

    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;

    

    esp_err_t errorCode = gpio_config(&io_conf);
    if (errorCode) {
        printf("Error configuring input pins");
    }

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(RIGHT_TACH1, right_encoder_isr_handler, (void*) RIGHT_TACH1);
    gpio_isr_handler_add(RIGHT_TACH2, right_encoder_isr_handler, (void*) RIGHT_TACH2);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(LEFT_TACH1, left_encoder_isr_handler, (void*) LEFT_TACH1);
    gpio_isr_handler_add(LEFT_TACH2, left_encoder_isr_handler, (void*) LEFT_TACH2);


    right_encoder_evt_queue = xQueueCreate(100, sizeof(float));
    left_encoder_evt_queue = xQueueCreate(100, sizeof(float));
    // xQueueSendFromISR(right_encoder_evt_queue, &gpio_num, NULL);

}

static void motor_set_pwm_right( float  duty_cycle ) {

    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);

    if (duty_cycle < 0) {
        duty_cycle += -1;
        gpio_set_level(RIGHT_MOTOR_ENABLE, 0);
    }
    else {
        gpio_set_level(RIGHT_MOTOR_ENABLE, 1);
    }

    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state

    // mcpwm_set_signal_high(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A);
    mcpwm_start(MCPWM_UNIT_0, MCPWM_TIMER_0);


}

static void motor_set_pwm_left( float duty_cycle) {

    mcpwm_set_signal_low(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B);
    if (duty_cycle < 0) {
        duty_cycle += -1;
        gpio_set_level(LEFT_MOTOR_ENABLE, 0);
    }  else {
        gpio_set_level(LEFT_MOTOR_ENABLE, 1);
    }
    mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, duty_cycle);
    mcpwm_set_duty_type(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, MCPWM_DUTY_MODE_0); //call this each time, if operator was previously in low/high state


}



static void PWM_initialize()
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, LEFT_PWM);
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, RIGHT_PWM);

    gpio_config_t io_conf;
    //disable interrupt
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = ((1ULL<<RIGHT_MOTOR_ENABLE) | (1ULL<<LEFT_MOTOR_ENABLE));
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 2000;    //frequency = 500Hz,
    pwm_config.cmpr_a = 25;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 75;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings


}







int newData = 0;
void motorForeground() {
    float io_num;

    BaseType_t  hasValues; 
    do {
        hasValues = xQueueReceive(right_encoder_evt_queue, &io_num, 0);
        if (hasValues) {
            right_position += io_num;
        }

    } while (hasValues);

    do {
        hasValues = xQueueReceive(left_encoder_evt_queue, &io_num, 0);
        if (hasValues) {
            left_position += io_num;
        }

    } while (hasValues);

}


int64_t currentTime = 0; 
int64_t previousTime =  0;
int64_t timeDiff =  0;
#define SAMPLING_PERIOD_MICRO_SEC 10000 // = 10 ms interrupt
// #define SAMPLING_PERIOD_MICRO_SEC 10000 // = 10 ms interrupt

typedef struct {
    float currentPos;
    float prevPos;
    float velocity;
} State; 

static float getCurrentPos_right() {
    return right_position;
}
static State state;

static void periodic_timer_callback(void* arg)
{


    newData = 1;
    currentTime = esp_timer_get_time();
    timeDiff = currentTime  - previousTime;
    previousTime = currentTime;
    state.prevPos = state.currentPos; 
    state.currentPos = getCurrentPos_right();
    state.velocity = ( state.currentPos - state.prevPos ) /10   ;
    // state.velocity = ( state.currentPos - state.prevPos )  / (timeDiff/ 100)   ;
    // state.velocity = ( state.currentPos - state.prevPos )  / ( timeDiff / 1000 ) ;




}


void timer_initialize() {

    velocity_evt_queue = xQueueCreate(100, sizeof(float));

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    /* The timer has been created but is not running yet */

    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, SAMPLING_PERIOD_MICRO_SEC));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000));

}
int everySec = 0;
float pwmDuty = 0;
void run_robot(void* arg ) {
    encoder_initialize();
    PWM_initialize();
    timer_initialize();

    right_position = 0;
    left_position = 0;
    motor_set_pwm_left( 100 );
    motor_set_pwm_right( 100 );

    gpio_set_level(RIGHT_MOTOR_ENABLE, 1);
    while (1) {
        motorForeground() ;
        if (newData){
            // printf("Left_Position: %f, Right_Position: %f, Right_vel: %f, Right_pos_diff: %f\n", 
            printf("Right_vel: %f\n", 
            // printf("Right_pos_cm: %f, Right_pos_prev_cm: %f, Right_vel: %f\n", 
                // state.currentPos,
                // state.prevPos,
                state.velocity
                );
                everySec++;
                if (everySec > 10)  {
                    everySec = 0;
                    pwmDuty += 1;
                    if (pwmDuty > 99) {
                        pwmDuty = 30;
                    }

                    motor_set_pwm_right( pwmDuty );
                }

            newData = 0;

         }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    xTaskCreate(run_robot, "Robot Foreground", 2048, NULL, 10, NULL);
    
}
