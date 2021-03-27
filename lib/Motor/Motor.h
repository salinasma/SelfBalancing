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
#define DEG_PER_TICK_MOTOR_SHAFT 30
#define TICKS_PER_ROTATION_MOTOR_SHAFT 12
#define DEG_PER_TICK_WHEEL 4
#define TICKS_PER_ROTATION_WHEEL 90
#define MOTOR_GEAR_RATIO 30 // 30 rotations of motor shaft for every 1 rotation of the wheel

class Motor {
    public:
        Motor (int pwmAPin, int pwmBPin);

        void rotatePwm (int pwmVal); 
        
        virtual int getRotationPeriodA (void); 

        virtual int getRotationPeriodB (void); 

    protected:
        static int pwmChannelSelector;
        static int timerSelector;
        int pwmA, pwmB, pwmChannelA, pwmChannelB, timerId; 
};

#endif

