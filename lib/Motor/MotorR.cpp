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

volatile int MotorR::stopMotor = 0;
volatile int MotorR::degrees = 0;

volatile int MotorR::tachA = 0;
volatile int MotorR::tachB = 0;

volatile uint8_t MotorR::fbState[2] = {0, 0}; // forward/backward state machine {a, b}
volatile uint8_t MotorR::fbOutput[2] = {1, 1}; // forward/backward state machine output {e, f}
volatile uint8_t MotorR::fbStateInit = 0;

volatile uint8_t MotorR::tachAlive = 0;
hw_timer_t* MotorR::tachSamplingTimer;

MotorR::MotorR (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval) 
: Motor {pwmAPin, pwmBPin} {
  pinMode (tachAPin, INPUT);
  pinMode (tachBPin, INPUT);

  tachA = tachAPin;
  tachB = tachBPin;

  tachSamplingTimer = timerBegin (timerId, 80, true); // each timer tick is 1 us

  // put interrupt and noInterrupt around all the init code
  noInterrupts ();

      attachInterrupt (digitalPinToInterrupt(tachAPin), tachAInterruptHandler, CHANGE);
      attachInterrupt (digitalPinToInterrupt(tachBPin), tachBInterruptHandler, CHANGE);
    
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

void MotorR::rotateDegrees (int degreesToRotate) {
    degrees = degreesToRotate;

    return;
}



void IRAM_ATTR MotorR::tachAInterruptHandler (void) {
    static int lastTime = micros();
    static int tickCount = 0;
    static int rotationCount = 0;
    static int degreesTickCount = 0;


    int currentTime;

    // detect forward/backward rotation
    uint8_t fbInputA = digitalRead(tachA) & 0x01;
    uint8_t fbInputB = digitalRead(tachB) & 0x01;

    if (!fbStateInit) {
        fbState[0] = fbInputA;
        fbState[1] = fbInputB;
        fbStateInit = 1;
    }
    
    // e 
    fbOutput[0] = (!fbState[1] && !fbInputA) || (!fbState[0] && fbInputB) || (fbState[1] && fbInputA) || (fbState[0] && !fbInputB);
    // f 
    fbOutput[1] = (!fbState[0] && !fbInputB) || (fbState[1] && !fbInputB) || (fbState[0] && fbInputB) || (!fbState[1] && fbInputA);
    fbState[0] = fbInputA;
    fbState[1] = fbInputB;
  

    // count ticks only on rising edge interrupt
    if (digitalRead(tachA)) {
        tachAlive = 1;

        noInterrupts ();
            tickCount++;
            degreesTickCount++;
        interrupts ();
    
            // measure shaft speed
            if (tickCount == TICKS_PER_ROTATION_WHEEL) {
                currentTime = micros();
        
                instPeriodA = currentTime - lastTime; 
                tachAAggr += instPeriodA;
                tachASampleCount++;
        
                lastTime = currentTime;
                
                tickCount = 0;
            }
    
            // measure shaft rotation
//            if (degreesTickCount * DEG_PER_TICK_WHEEL >= degrees) {
//               degreesTickCount = 0; 
//               stopMotor = 1;
//            }
    }

    return;
}

void IRAM_ATTR MotorR::tachBInterruptHandler(void) {
    static int lastTime = micros();
    static int tickCount = 0;
    static int rotationCount = 0;

    int currentTime;

    // detect forward/backward rotation
    uint8_t fbInputA = digitalRead(tachA) & 0x01;
    uint8_t fbInputB = digitalRead(tachB) & 0x01;

    if (!fbStateInit) {
        fbState[0] = fbInputA;
        fbState[1] = fbInputB;
        fbStateInit = 1;
    }
    
    // e 
    fbOutput[0] = (!fbState[1] && !fbInputA) || (!fbState[0] && fbInputB) || (fbState[1] && fbInputA) || (fbState[0] && !fbInputB);
    // f 
    fbOutput[1] = (!fbState[0] && !fbInputB) || (fbState[1] && !fbInputB) || (fbState[0] && fbInputB) || (!fbState[1] && fbInputA);
    fbState[0] = fbInputA;
    fbState[1] = fbInputB;
         
    // count ticks only on rising edge interrupt
    if (digitalRead(tachB)) {
        noInterrupts();
            tickCount++;
        interrupts();

        if (tickCount == 12) {
            currentTime = micros();

            instPeriodB = currentTime - lastTime; 
            tachBAggr += instPeriodB;
            tachBSampleCount++;

            lastTime = currentTime;
            
            tickCount = 0;
        }
    }
  
    return;
}

void IRAM_ATTR MotorR::tachSamplingTimerInterrupt (){
//    if (tachASampleCount != 0) {
//        avgPeriodA = tachAAggr / tachASampleCount;
//        tachAAggr = 0;
//        tachASampleCount = 0;
//    }
//
//    if (tachBSampleCount != 0) {
//        avgPeriodB = tachBAggr / tachBSampleCount;
//        tachBAggr = 0;
//        tachBSampleCount = 0;
//    }

    if (tachAlive) {
        tachAlive = 0;
        return;
    }

    instPeriodA = 0;
}

