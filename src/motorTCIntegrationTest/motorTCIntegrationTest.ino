#include <TCClient.h>

#include <MotorL.h>
#include <MotorR.h>
#include <Imu.h>
#include <controls.hpp>
#include <filters.h>

#define RIGHT_PWM1 26
#define RIGHT_PWM2 27
#define RIGHT_TACH1 18
#define RIGHT_TACH2 19
#define RIGHT_TACH_SAMPLE_INTERVAL 1000 // microseconds

#define LEFT_PWM1 25
#define LEFT_PWM2 33
#define LEFT_TACH1 34
#define LEFT_TACH2 35
#define LEFT_TACH_SAMPLE_INTERVAL 1000 // microseconds

#define CUTOFF_FREQ 10  // Hz
#define TACH_SAMPLE_INTERVAL_SECONDS 0.001 // seconds
#define FILTER_ORDER  IIR::ORDER::OD1 // OD1 to OD4

#define NUM_PARAMS 7
#define NUM_DATA 6

MotorL left (LEFT_PWM1, LEFT_PWM2, LEFT_TACH1, LEFT_TACH2, LEFT_TACH_SAMPLE_INTERVAL);
Controls leftMotorPID (2, .09, .75, pi, 0, 1470);
Filter leftFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);
Controls rightMotorPID (2, 12.1, .75, pi, 0, 1470);
Filter rightFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

Imu imu (32);

int lPeriod = 0;
int lFrequency = 0;
int lPwm = 0;

int rPeriod = 0;
int rFrequency = 0;
int rPwm = 0;

hw_timer_t* controlLoopTimer; 

void controlLoop(void) {
    lPeriod = left.getInstPeriodA();
    if (lPeriod != 0) { 
        lFrequency = 1000000/lPeriod;
    } else {
        lFrequency = 0;
    }
//    lFrequency = leftFilter.filterIn((float)lFrequency);
    lPwm = leftMotorPID.controlWorkLoop (lFrequency, 50);

    rPeriod = right.getInstPeriodA ();
    if (rPeriod != 0) {
        rFrequency = 1000000/rPeriod; // Hz or rotations-per-second
    } else {
        rFrequency = 0;
    }
//    rFrequency = rightFilter.filterIn((float)rFrequency);
    rPwm = rightMotorPID.controlWorkLoop (rFrequency, 50);
}

int httpCode;
TCClient *botClient;

int bobble = 255;

// name, pointer, regIndex, dataType, dataSize 
Data paramReg[NUM_PARAMS] = {{"bobble", &bobble, 0, INT, sizeof(bobble)},
                             {"RM kP", &(rightMotorPID.kP), 1, DOUBLE, sizeof(rightMotorPID.kP)},
                             {"RM kI", &(rightMotorPID.kI), 2, DOUBLE, sizeof(rightMotorPID.kI)},
                             {"RM kD", &(rightMotorPID.kD), 3, DOUBLE, sizeof(rightMotorPID.kD)},
                             {"LM kP", &(leftMotorPID.kP), 4, DOUBLE, sizeof(leftMotorPID.kP)},
                             {"LM kI", &(leftMotorPID.kI), 5, DOUBLE, sizeof(leftMotorPID.kI)},
                             {"LM kD", &(leftMotorPID.kD), 6, DOUBLE, sizeof(leftMotorPID.kD)}
                            };
Data dataReg[NUM_DATA] = {{"lPeriod", &lPeriod, 0, INT, sizeof(lPeriod)},
                          {"lFrequency", &lFrequency, 1, INT, sizeof(lFrequency)},
                          {"lPwm", &lPwm, 2, INT, sizeof(lPwm)},
                          {"rPeriod", &rPeriod, 3, INT, sizeof(rPeriod)},
                          {"rFrequency", &rFrequency, 4, INT, sizeof(rFrequency)},
                          {"rPwm", &rPwm, 5, INT, sizeof(rPwm)}
                         };

void setup() {
  Serial.begin(9600);
  while (!Serial);

  botClient = new TCClient("ssid", "password", "address", paramReg, NUM_PARAMS, dataReg, NUM_DATA);
  Serial.println("Hello");

  do {
    httpCode = botClient->postInitParamRegistry();
  } while (httpCode == 0);

  do {
    httpCode = botClient->postInitDataRegistry();
  } while (httpCode == 0);


  noInterrupts();
    controlLoopTimer = timerBegin(0, 80, true); // each timer tick is 1 us                                                             
    timerAttachInterrupt(controlLoopTimer, controlLoop, true);
    timerAlarmWrite(controlLoopTimer, 1000, true); // 1000 microseconds
    timerAlarmEnable(controlLoopTimer);
  interrupts();
  
  left.rotate (70);
  right.rotate (70);
}

void loop() {
  Serial.println("rPeriod: " + String(rPeriod));
  left.rotate(lPwm);
  right.rotate(rPwm);
  
  if (botClient->isConnectedToWifi()) {
    Serial.println("Sending new data");
    for (int i = 0; i < NUM_DATA; i++) {
      httpCode = botClient->postUpdateData(i);
      botClient->parseJSONResponse(httpCode, UPDATE_DATA);
    }

    Serial.println("Getting new params");
    httpCode = botClient->postGetParams();
    botClient->parseJSONResponse(httpCode, GET_PARAMS);
    
    Serial.println("Sending new params");
    httpCode = botClient->postInitParamRegistry();
    botClient->parseJSONResponse(httpCode, INIT_PARAMS);
  }
  
  delay(500);
}
