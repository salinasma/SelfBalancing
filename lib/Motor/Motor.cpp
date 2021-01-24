/*
 * Author: Pramuka Perera
 * Description: Software interface for Polulu Part 3072 Micro-metal Gearmotor 
 */

#include "Motor.h"

int Motor::pwmChannelSelector = 0;
int Motor::timerSelector = 0;

Motor::Motor (int pwmAPin, int pwmBPin) {
    pwmA = pwmAPin;
    pwmB = pwmBPin;

    pwmChannelA = pwmChannelSelector;
    ledcSetup (pwmChannelA, PWM_FREQ, PWM_RES); 
    ledcAttachPin (pwmA, pwmChannelA);
    pwmChannelSelector = (pwmChannelSelector + 1) % NUM_PWM_CHANNELS;

    pwmChannelB = pwmChannelSelector;
    ledcSetup (pwmChannelB, PWM_FREQ, PWM_RES); 
    ledcAttachPin (pwmB, pwmChannelB);
    pwmChannelSelector = (pwmChannelSelector + 1) % NUM_PWM_CHANNELS;

    pinMode (pwmA, OUTPUT);
    pinMode (pwmB, OUTPUT);

    digitalWrite (pwmA, 0);
    digitalWrite (pwmB, 0);

    timerId = timerSelector;
    timerSelector++;
}

void Motor::rotatePwm (int pwmVal) {

    if (pwmVal <= 10 && pwmVal >= -10) {
        digitalWrite (pwmB, 0);
        ledcWrite (pwmChannelA, 10);

        return;
    }

    // rotate backward
    if (pwmVal < 0) {
        pwmVal *= -1;
        if (pwmVal > 256) {
            pwmVal = 250;
        }
        digitalWrite (pwmA, 1);
        ledcWrite (pwmChannelB, pwmVal);

        return;
    }

    // rotate forward
    if (pwmVal > 256) {
        pwmVal = 250;
    }
    digitalWrite (pwmB, 0);
    ledcWrite (pwmChannelA, pwmVal);

    return;
}

int Motor::getRotationPeriodA (void) {
    return 0;
}

int Motor::getRotationPeriodB (void) {
    return 0;
}

