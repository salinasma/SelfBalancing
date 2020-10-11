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

hw_timer_t *MotorL::tachSamplingTimer;
static int lastTime = 0;
float lastPosition = 0;
float lastVelocity = 0;


float timeStep;
// static MotorL::MotorL MLeft;
// MotorL::MotorL getMLeft(void ){
//     reutrn  MLeft;
// } 
// MotorL::MotorL (int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval, controlMode mode)
MotorL::MotorL(int pwmAPin, int pwmBPin, int tachAPin, int tachBPin, int tachSampleInterval)
    : Motor{pwmAPin, pwmBPin}
{
    pinMode(tachAPin, INPUT);
    pinMode(tachBPin, INPUT);


    a_pin = tachAPin;
    b_pin = tachBPin;
    timeStep =  (float) tachSampleInterval;

    tachSamplingTimer = timerBegin(timerId, 80, true); // each timer tick is 1 us

    // put interrupt and noInterrupt around all the init code
    noInterrupts();

    attachInterrupt(digitalPinToInterrupt(tachAPin), tachAInterruptHandler, CHANGE);
    attachInterrupt(digitalPinToInterrupt(tachBPin), tachBInterruptHandler, CHANGE);

    // attachInterrupt(digitalPinToInterrupt(tachAPin), tachAInterruptHandler, RISING);
    // attachInterrupt(digitalPinToInterrupt(tachBPin), tachBInterruptHandler, RISING);

    timerAttachInterrupt(tachSamplingTimer, &tachSamplingTimerInterrupt, true);

    timerAlarmWrite(tachSamplingTimer, tachSampleInterval, true);
    timerAlarmEnable(tachSamplingTimer);

    interrupts();
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

State MotorL::getMotorState(void)
{
    return motorState;
}

// void MotorL::init(void)  {

// }

MotorL::FSM_state FSM[4] = {
    {{MotorL::Stopped, MotorL::Backwards, MotorL::Forwards, MotorL::Invalid}, {0, 1, 2, 0}}, // 00
    {{MotorL::Forwards, MotorL::Stopped, MotorL::Invalid, MotorL::Backwards}, {0, 1, 1, 3}}, // 01
    {{MotorL::Backwards, MotorL::Invalid, MotorL::Stopped, MotorL::Forwards}, {0, 2, 2, 3}}, // 10
    {{MotorL::Invalid, MotorL::Forwards, MotorL::Backwards, MotorL::Stopped}, {3, 1, 2, 3}}  // 11
};

void posCode()
{
    int a = digitalRead(a_pin);
    int b = digitalRead(b_pin);
    int input = (a * 2) + b;
    int output = FSM[FSM_current_state].out[input];


    FSM_current_state = FSM[FSM_current_state].next[input];
    switch (output)
    {
    case MotorL::Stopped:
        break;
    case MotorL::Forwards:
        motorState.fw++;
        motorState.position += STEP_SIZE_cm;
        break;
    case MotorL::Backwards:
        motorState.position -= STEP_SIZE_cm;
        motorState.bw++;
        break;
    default:
        motorState.invalidCount++;
        break;
    }
}

void IRAM_ATTR MotorL::tachAInterruptHandler(void)
{
    static int lastTime = 0;
    int currentTime;

    noInterrupts();

    // set to some timer's current val
    currentTime = micros();

    instPeriodA = currentTime - lastTime;
    tachAAggr += instPeriodA;
    tachASampleCount++;

    lastTime = currentTime;
    posCode();

    interrupts();

    return;
}

void IRAM_ATTR MotorL::tachBInterruptHandler(void)
{
    static int lastTime = 0;
    // set to some timer's current val

    noInterrupts();

    int currentTime = micros();

    instPeriodB = currentTime - lastTime;
    tachBAggr += instPeriodB;
    tachBSampleCount++;

    lastTime = currentTime;

    posCode();

    interrupts();

    return;
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
