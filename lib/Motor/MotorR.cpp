/*
 * Author: Pramuka Perera
 * Description: Right motor implementation 
 */

#include "MotorR.h"

volatile int MotorR::instPeriodA = 0;
volatile int MotorR::avgPeriodA = 0;
volatile int MotorR::tachAAggr = 0;
volatile int MotorR::tachASampleCount = 0;

volatile int MotorR::instPeriodB = 0;
volatile int MotorR::avgPeriodB = 0;
volatile int MotorR::tachBAggr = 0;
volatile int MotorR::tachBSampleCount = 0;

hw_timer_t* MotorR::tachSamplingTimer;

MotorR::MotorR (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval) 
: Motor {pwmAPin, pwmBPin} {
  pinMode (tachAPin, INPUT);
  pinMode (tachBPin, INPUT);

  tachSamplingTimer = timerBegin (timerId, 80, true); // each timer tick is 1 us

  // put interrupt and noInterrupt around all the init code
  noInterrupts ();

      attachInterrupt (digitalPinToInterrupt(tachAPin), tachAInterruptHandler, RISING);
      attachInterrupt (digitalPinToInterrupt(tachBPin), tachBInterruptHandler, RISING);
    
      timerAttachInterrupt (tachSamplingTimer, &tachSamplingTimerInterrupt, true);
      timerAlarmWrite (tachSamplingTimer, tachSampleInterval, true);
      timerAlarmEnable (tachSamplingTimer);

  interrupts ();
}

int MotorR::getInstPeriodA (void) {
    return instPeriodA;
}
 
int MotorR::getInstPeriodB (void) {
    return instPeriodB;
}

int MotorR::getAvgPeriodA (void) {
    return avgPeriodA;
}
 
int MotorR::getAvgPeriodB (void) {
    return avgPeriodB;
}

void IRAM_ATTR MotorR::tachAInterruptHandler (void) {
     static int lastTime = 0;
     int currentTime;

     noInterrupts ();

         // set to some timer's current val
         currentTime = micros ();

         instPeriodA = currentTime - lastTime; 
         tachAAggr += instPeriodA;
         tachASampleCount++;

         lastTime = currentTime;

     interrupts ();
  
     return;
}

void IRAM_ATTR MotorR::tachBInterruptHandler (void) {
    static int lastTime = 0;
    // set to some timer's current val
    int currentTime;
         
    noInterrupts ();

        currentTime = micros ();

        instPeriodB = currentTime - lastTime; 
        tachBAggr += instPeriodB;
        tachBSampleCount++;

        lastTime = currentTime;

    interrupts ();
  
    return;
}

void IRAM_ATTR MotorR::tachSamplingTimerInterrupt (){
    if (tachASampleCount != 0) {
        avgPeriodA = tachAAggr / tachASampleCount;
        tachAAggr = 0;
        tachASampleCount = 0;
    }

    if (tachBSampleCount != 0) {
        avgPeriodB = tachBAggr / tachBSampleCount;
        tachBAggr = 0;
        tachBSampleCount = 0;
    }
}

