/*
 * Author: Pramuka Perera
 * Description: Left motor implementation 
 */

#include "MotorL.h"

volatile int MotorL::instPeriodA = 0;
volatile int MotorL::avgPeriodA = 0;
volatile int MotorL::tachAAggr = 0;
volatile int MotorL::tachASampleCount = 0;

volatile int MotorL::instPeriodB = 0;
volatile int MotorL::avgPeriodB = 0;
volatile int MotorL::tachBAggr = 0;
volatile int MotorL::tachBSampleCount = 0;

hw_timer_t* MotorL::tachSamplingTimer;

MotorL::MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval) 
: Motor {pwmAPin, pwmBPin} {
  pinMode (tachAPin, INPUT);
  pinMode (tachBPin, INPUT);
  
  tachSamplingTimer = timerBegin (timerId, 80, true); // each timer tick is 1 us

  // put interrupt and noInterrupt around all the init code
  noInterrupts ();

      attachInterrupt (digitalPinToInterrupt (tachAPin), tachAInterruptHandler, RISING);
      attachInterrupt (digitalPinToInterrupt (tachBPin), tachBInterruptHandler, RISING);
    
      timerAttachInterrupt (tachSamplingTimer, &tachSamplingTimerInterrupt, true);
      timerAlarmWrite (tachSamplingTimer, tachSampleInterval, true);
      timerAlarmEnable (tachSamplingTimer);

  interrupts ();
}

int MotorL::getInstPeriodA (void) {
    return instPeriodA;
}
 
int MotorL::getInstPeriodB (void) {
    return instPeriodB;
}

int MotorL::getAvgPeriodA (void) {
    return avgPeriodA;
}
 
int MotorL::getAvgPeriodB (void) {
    return avgPeriodB;
}

void IRAM_ATTR MotorL::tachAInterruptHandler (void) {
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

void IRAM_ATTR MotorL::tachBInterruptHandler (void) {
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

void IRAM_ATTR MotorL::tachSamplingTimerInterrupt (){
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

