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

volatile int MotorL::tachLogger[TACH_LOGGER_SIZE] = {0};
volatile int MotorL::tachLoggerIndex = 0;

volatile int MotorL::stopMotor = 0;
volatile int MotorL::degrees = 0;

volatile int MotorL::tachA = 0;
volatile int MotorL::tachB = 0;

volatile uint8_t MotorL::fbState[2] = {0, 0}; // forward/backward state machine {a, b}
volatile uint8_t MotorL::fbOutput[2] = {1, 1}; // forward/backward state machine output {e, f}
volatile uint8_t MotorL::fbStateInit = 0;

volatile uint8_t MotorL::tachAlive = 0;
hw_timer_t* MotorL::tachSamplingTimer;

MotorL::MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval) 
: Motor {pwmAPin, pwmBPin} {
  pinMode (tachAPin, INPUT);
  pinMode (tachBPin, INPUT);

  tachA = tachAPin;
  tachB = tachBPin;

//  Serial.begin (9600);
  
  tachSamplingTimer = timerBegin (timerId, 80, true); // each timer tick is 1 us

  // put interrupt and noInterrupt around all the init code
  noInterrupts ();

      attachInterrupt (digitalPinToInterrupt (tachAPin), tachAInterruptHandler, CHANGE);
      attachInterrupt (digitalPinToInterrupt (tachBPin), tachBInterruptHandler, CHANGE);
    
      timerAttachInterrupt (tachSamplingTimer, &tachSamplingTimerInterrupt, true);
      timerAlarmWrite (tachSamplingTimer, tachSampleInterval, true);
      timerAlarmEnable (tachSamplingTimer);

  interrupts ();
}

int MotorL::getInstPeriodA(void)
{
    return instPeriodA;
}

int MotorL::getInstPeriodB(void)
{
    return instPeriodB;
}

int MotorL::getAvgPeriodA(void)
{
    return avgPeriodA;
}

int MotorL::getAvgPeriodB(void)
{
    return avgPeriodB;
}

void MotorL::rotateDegrees (int degreesToRotate) {
    degrees = degreesToRotate;

    return;
}

void IRAM_ATTR MotorL::tachAInterruptHandler (void) {
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

void IRAM_ATTR MotorL::tachBInterruptHandler(void) {
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
        tickCount++;

        if (tickCount == 12) {
            currentTime = micros();

            instPeriodB = currentTime - lastTime; 
//            tachBAggr += instPeriodB;
//            tachBSampleCount++;

            lastTime = currentTime;
            
            tickCount = 0;
        }
    }
  
    return;
}

void IRAM_ATTR MotorL::tachSamplingTimerInterrupt (){
//    if (tachASampleCount != 0) {
//        avgPeriodA = tachAAggr / tachASampleCount;
//        tachAAggr = 0;
//        tachASampleCount = 0;
//
//        tachLoggerIndex++;
//        if (tachLoggerIndex < TACH_LOGGER_SIZE) {
//            tachLogger[tachLoggerIndex] = avgPeriodA;
//        }
//    }
//
//    if (tachBSampleCount != 0) {
//        avgPeriodB = tachBAggr / tachBSampleCount;
//        tachBAggr = 0;
//        tachBSampleCount = 0;
//    }
//
    if (tachAlive) {
        tachAlive = 0;
        return;
    }

    instPeriodA = 0;
}

void IRAM_ATTR MotorL::tachSamplingTimerInterrupt()
{
    // MotorL* m = getMLeft(void );

    // noInterrupts();
    // int currentTime = micros();

    // if (tachASampleCount != 0)
    // {
    //     avgPeriodA = tachAAggr / tachASampleCount;
    //     tachAAggr = 0;
    //     tachASampleCount = 0;

    //     tachLoggerIndex++;
    //     if (tachLoggerIndex < TACH_LOGGER_SIZE)
    //     {
    //         tachLogger[tachLoggerIndex] = avgPeriodA;
    //     }
    // }

    // if (tachBSampleCount != 0)
    // {
    //     avgPeriodB = tachBAggr / tachBSampleCount;
    //     tachBAggr = 0;
    //     tachBSampleCount = 0;
    // }

    // motorState.velocity =  ( motorState.position - lastPosition  ); ///   ((float)timeStep ) ;
    //motorState.velocity =  motorState.position ; ///   ((float)timeStep ) ;
    motorState.velocity = motorState.position ; ///   ((float)timeStep ) ;
    // motorState.acceleration=  ( motorState.velocity  - lastVelocity  ) / timeStep;
    // lastVelocity =  motorState.velocity ;
    // lastPosition =  motorState.position;
    // lastTime = currentTime;

    // interrupts();
}
