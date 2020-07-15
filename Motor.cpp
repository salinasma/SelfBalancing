/*
 * Author: Pramuka Perera
 * Description: Software interface for Polulu Part 3072 Micro-metal Gearmotor 
 */

#include "Motor.h"

void Motor::Motor (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin) {

    pwmA = pwmPin;
    pwmB = pwmPin;
    tachA = tachAPin;
    tachB = tachBPin;

    pinMode (pwmA, OUTPUT);
    pinMode (pwmB, OUTPUT);
    pinMode (tachA, INPUT);
    pinMode (tachB, INPUT);

    digitalWrite (pwmA, 1);
    digitalWrite (pwmB, 1);

    // put interrupt and noInterrupt around all the init code
    noInterrupts ();
    attachInterrupt (digitalPinToInterrupt(tachA), tachAInterruptHandler, RISING);
    attachInterrupt (digitalPinToInterrupt(tachB), tachBInterruptHandler, RISING);
    interrupts ();
}

void Motor::rotate (int rpm) {

    if (rpm == 0) {
      digitalWrite (pwmA, 1);  
      digitalWrite (pwmB, 1);  
    }

    // math to convert rpm to 0-255 scale
    int pwmVal;

    // rotate backward
    if (rpm < 0) {
        digitalWrite (pwmAPin, 0);
        digitalWrite (pwmBPin, pwmVal);

        return;
    }

    // rotate forward
    digitalWrite (pwmBPin, 0);
    digitalWrite (pwmAPin, pwmVal);

    return;
}

void Motor:: neutral (int rpm) {

    digitalWrite (pwmAPin, 0);
    digitalWrite (pwmBPin, 0);

    return;
}

int Motor:: getRpm (void) {
    return ;
}

void Motor:: tachAInterruptHandler (void) {
    static int lastTime = 0;
    // set to some timer's current val
    int currentTime;

    tachAMeasuredTimePeriod = currentTime - lastTime; 
    lastTime = currentTime;

    return;
}


void Motor:: tachBInterruptHandler (void) {
    static int lastTime = 0;
    // set to some timer's current val
    int currentTime;

    tachBMeasuredTimePeriod = currentTime - lastTime; 
    lastTime = currentTime;

    return;
}
