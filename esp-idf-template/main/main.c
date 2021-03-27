#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_timer.h"

#define RIGHT_TACH1 18
#define RIGHT_TACH2 19

#define LEFT_TACH1 34
#define LEFT_TACH2 35
#define LEFT_TACH_SAMPLE_INTERVAL 1000 // microseconds

#define ESP_INTR_FLAG_DEFAULT 0


#define GPIO_INPUT_PIN_SEL  ((1ULL<<RIGHT_TACH1) | (1ULL<<RIGHT_TACH2) | (1ULL<<LEFT_TACH1) | (1ULL<<LEFT_TACH2))


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
            returnValue =  STEP_SIZE_mm;
            break;
        case Backwards:
            returnValue =   -1* STEP_SIZE_mm;
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
#define SAMPLING_PERIOD 10000

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
    state.velocity = ( state.currentPos - state.prevPos ) ;// / ( timeDiff / 1000 ) ;
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

    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, SAMPLING_PERIOD));
    // ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 1000));

}

void run_robot(void* arg ) {
    encoder_initialize();
    timer_initialize();

    right_position = 0;
    left_position = 0;
    while (1) {
        motorForeground() ;
        if (newData){
            // printf("Left_Position: %f, Right_Position: %f, Right_vel: %f, Right_pos_diff: %f\n", 
            printf("Right_pos_cm: %f, Right_pos_prev_cm: %f, Right_vel: %f\n", 
                state.currentPos,
                state.prevPos,
                state.velocity
                );

            newData = 0;

         }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{

    xTaskCreate(run_robot, "Robot Foreground", 2048, NULL, 10, NULL);
    
}
