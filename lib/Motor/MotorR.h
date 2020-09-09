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


    private:
        static hw_timer_t* tachSamplingTimer;
        static volatile int tachAAggr, tachBAggr, tachASampleCount, tachBSampleCount;
        static volatile int instPeriodA, avgPeriodA, instPeriodB, avgPeriodB;
        
        static void IRAM_ATTR tachAInterruptHandler (void);
        
        static void IRAM_ATTR tachBInterruptHandler (void);

        static void IRAM_ATTR tachSamplingTimerInterrupt (void);
};

#endif

