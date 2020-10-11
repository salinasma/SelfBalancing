/*
 * Author: Pramuka Perera
 * Description: Left motor implementation 
 */

#include "Motor.h"

#ifndef MOTOR_L_H
#define MOTOR_L_H

#define TACH_LOGGER_SIZE 1000

#define TACH_CNT_PER_CYCLE 12.0
#define GEAR_RATIO 30.0
#define PI 3.1415
#define WHEEL_DIAMETER_CM 9.0 // cm
#define wheel_circumference  PI *  WHEEL_DIAMETER_CM
#define STEP_SIZE_cm  WHEEL_DIAMETER_CM /   (TACH_CNT_PER_CYCLE * GEAR_RATIO ) 

typedef struct state {
    float acceleration;
    float velocity;
    float position;
    int bw;
    int fw;
    int invalidCount;
} State ;



static State motorState;
static int FSM_current_state;
static int a_pin;
static int b_pin;


class MotorL : public Motor {   
    public:
        // enum controlMode {POSITION, VELOCITY, ACCELERATION }; 

        int currentState = 0;
        enum Quad_out{ Stopped = 0, Forwards = 1, Backwards =2, Invalid = 3 };
        typedef struct fsmState {
            Quad_out out[4];
            int next[4];
        } FSM_state ;

        static volatile int tachLogger[TACH_LOGGER_SIZE];
        static volatile int tachLoggerIndex;
        
        // MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval, controlMode mode); 
        MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval); 
    
        int getInstPeriodA (void); 
     
        int getInstPeriodB (void); 

        int getAvgPeriodA (void);

        int getAvgPeriodB (void);

        State getMotorState (void);

        void init(void);


    private:
        static hw_timer_t* tachSamplingTimer;
        static volatile int tachAAggr, tachBAggr, tachASampleCount, tachBSampleCount;
        static volatile int instPeriodA, avgPeriodA, instPeriodB, avgPeriodB;

        static void IRAM_ATTR tachAInterruptHandler (void);
        
        static void IRAM_ATTR tachBInterruptHandler (void);

        static void IRAM_ATTR tachSamplingTimerInterrupt (void);
};

#endif

