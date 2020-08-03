/*
 * Author: Pramuka Perera
 * Description: Right motor implementation 
 */

#include "Motor.h"

#ifndef MOTOR_R_H
#define MOTOR_R_H

class MotorR : public Motor {   
    public:
        MotorR (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin); 
    
        int getRotationPeriodA (void); 
     
        int getRotationPeriodB (void); 

    private:
        static int tachATime, tachBTime;
        int tachA, tachB;
        
        static void IRAM_ATTR tachAInterruptHandler (void);
        
        static void IRAM_ATTR tachBInterruptHandler (void);
};

#endif
