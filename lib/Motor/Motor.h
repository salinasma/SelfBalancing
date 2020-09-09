/*
 * Author: Pramuka Perera
 * Description: Software interface for Polulu Part 3072 Micro-metal Gearmotor 
 */

#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

#define WHEEL_DIAM_MM 90
#define PWM_FREQ 2000
#define PWM_RES 8
#define NUM_PWM_CHANNELS 16

class Motor {
    public:
        Motor (int pwmAPin, int pwmBPin);

        void rotate (int rpm); 
        
        virtual int getRotationPeriodA (void); 

        virtual int getRotationPeriodB (void); 

    protected:
        static int pwmChannelSelector;
        static int timerSelector;
        int pwmA, pwmB, pwmChannelA, pwmChannelB, timerId; 
};

#endif

