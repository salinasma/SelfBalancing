/*
 * Author: Pramuka Perera
 * Description: Left motor implementation 
 */

#include "Motor.h"

#ifndef MOTOR_L_H
#define MOTOR_L_H

#define TACH_LOGGER_SIZE 1000

class MotorL : public Motor {   
    public:
        static volatile int tachLogger[TACH_LOGGER_SIZE];
        static volatile int tachLoggerIndex;
        
        MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval); 
    
        int getInstPeriodA (void); 
     
        int getInstPeriodB (void); 

        int getAvgPeriodA (void);

        int getAvgPeriodB (void);


    private:
        static hw_timer_t* tachSamplingTimer;
        static volatile int tachAAggr, tachBAggr, tachASampleCount, tachBSampleCount;
        static volatile int instPeriodA, avgPeriodA, instPeriodB, avgPeriodB;

        static void IRAM_ATTR tachAInterruptHandler (void);
        
        static void IRAM_ATTR tachBInterruptHandler (void);

        static void IRAM_ATTR tachSamplingTimerInterrupt (void);
};

#endif

