/*
 * Author: Pramuka Perera
 * Description: Right motor implementation 
 */

#include "Motor.h"

#ifndef MOTOR_R_H
#define MOTOR_R_H

class MotorR : public Motor {   
    public:
        MotorR (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval); 
    
        int getInstPeriodA (void); 
     
        int getInstPeriodB (void); 

        int getAvgPeriodA (void);

        int getAvgPeriodB (void);

        void rotateDegrees (int degreesToRotate); 

        static volatile int stopMotor;
        static volatile uint8_t fbState[2]; // forward/backward state machine
        static volatile uint8_t fbOutput[2]; // forward/backward state machine output
        static volatile uint8_t fbStateInit; // flag that indicates if state has been initialized


    private:
        static hw_timer_t* tachSamplingTimer;
        static volatile int tachAAggr, tachBAggr, tachASampleCount, tachBSampleCount;
        static volatile int instPeriodA, avgPeriodA, instPeriodB, avgPeriodB;
        static volatile int degrees;
        static volatile int tachA, tachB;
        static volatile uint8_t tachAlive;
        
        static void IRAM_ATTR tachAInterruptHandler (void);
        
        static void IRAM_ATTR tachBInterruptHandler (void);

        static void IRAM_ATTR tachSamplingTimerInterrupt (void);
};

#endif

