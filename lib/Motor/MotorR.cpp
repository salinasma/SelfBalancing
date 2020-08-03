/*
 * Author: Pramuka Perera
 * Description: Right motor implementation 
 */

#include "MotorR.h"

int MotorR::tachATime = 0;
int MotorR::tachBTime = 0;

MotorR::MotorR (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin) 
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

int MotorR::getRotationPeriodA (void) {
    return tachATime;
}
 
int MotorR::getRotationPeriodB (void) {
    return tachBTime;
}

void IRAM_ATTR MotorR::tachAInterruptHandler (void) {
     static int lastTime = 0;
     // set to some timer's current val
     int currentTime = micros();
  
     tachATime = currentTime - lastTime; 
     lastTime = currentTime;
  
     return;
}

void IRAM_ATTR MotorR::tachBInterruptHandler (void) {
     static int lastTime = 0;
     // set to some timer's current val
     int currentTime = micros();
  
     tachBTime = currentTime - lastTime; 
     lastTime = currentTime;
  
     return;
}
