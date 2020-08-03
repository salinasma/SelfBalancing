/*
 * Author: Pramuka Perera
 * Description: Left motor implementation 
 */

#include "MotorL.h"

int MotorL::tachATime = 0;
int MotorL::tachBTime = 0;

MotorL::MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin) 
: Motor {pwmAPin, pwmBPin} {
  tachA = tachAPin;
  tachB = tachBPin;  
  pinMode (tachA, INPUT);
  pinMode (tachB, INPUT);
  
  // put interrupt and noInterrupt around all the init code
  noInterrupts ();
  attachInterrupt (digitalPinToInterrupt(tachA), tachAInterruptHandler, RISING);
  attachInterrupt (digitalPinToInterrupt(tachB), tachBInterruptHandler, RISING);
  interrupts ();
}

int MotorL::getRotationPeriodA (void) {
    return tachATime;
}
 
int MotorL::getRotationPeriodB (void) {
    return tachBTime;
}

void IRAM_ATTR MotorL::tachAInterruptHandler (void) {
     static int lastTime = 0;
     // set to some timer's current val
     int currentTime = micros();
  
     tachATime = currentTime - lastTime; 
     lastTime = currentTime;
  
     return;
}

void IRAM_ATTR MotorL::tachBInterruptHandler (void) {
     static int lastTime = 0;
     // set to some timer's current val
     int currentTime = micros();
  
     tachBTime = currentTime - lastTime; 
     lastTime = currentTime;
  
     return;
}
