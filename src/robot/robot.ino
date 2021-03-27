#include <TCClient.h>
#include <MotorL.h>
#include <MotorR.h>
#include <Imu.h>
#include <controls.hpp>
#include <filters.h>

#define MOTOR_CIRCUMFERENCE (double) 28.26

#define IMU_INT 4
#define IMU_XACC 0
#define IMU_YACC 1
#define IMU_ZACC 2
#define IMU_XROT 4
#define IMU_YROT 5
#define IMU_ZROT 6

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

#define NUM_PARAMS 9
#define NUM_DATA 12

/*-----Motor Control-----*/

MotorL left (LEFT_PWM1, LEFT_PWM2, LEFT_TACH1, LEFT_TACH2, LEFT_TACH_SAMPLE_INTERVAL);
Controls leftMotorPID (1, .09, .75, pi, -32768, 32767);
//Filter leftFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);
Controls rightMotorPID (1, .09, .75, pi, -32768, 32767);
//Filter rightFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

int lPeriod = 0;
int lFrequency = 0;
int lPwm = 0;

int rPeriod = 0;
int rFrequency = 0;
int rPwm = 0;

int rightTargetFrequency = 50;
int leftTargetFrequency = 50;

hw_timer_t* controlLoopTimer; 

void controlLoop() {  
    lPeriod = left.getInstPeriodA();
    if (lPeriod != 0) { 
        lFrequency = 1000000/lPeriod;
    } else {
        lFrequency = 0;
    }
//    lFrequency = leftFilter.filterIn((float)lFrequency);
    lPwm = leftMotorPID.controlWorkLoop (lFrequency, leftTargetFrequency);

    rPeriod = right.getInstPeriodA ();
    if (rPeriod != 0) {
        rFrequency = 1000000/rPeriod; // Hz or rotations-per-second
    } else {
        rFrequency = 0;
    }
//    rFrequency = rightFilter.filterIn((float)rFrequency);
    rPwm = rightMotorPID.controlWorkLoop (rFrequency, rightTargetFrequency);

    left.rotatePwm(lPwm);
    right.rotatePwm(rPwm);
}

/*-----TCClient-----*/

int httpCode;
TCClient *botClient;

int heartbeat = 0;

// name, pointer, regIndex, dataType, dataSize 
Data paramReg[NUM_PARAMS] = {{"heartbeat", &heartbeat, 0, INT, sizeof(heartbeat)},
                             {"RM kP", &(rightMotorPID.kP), 1, DOUBLE, sizeof(rightMotorPID.kP)},
                             {"RM kI", &(rightMotorPID.kI), 2, DOUBLE, sizeof(rightMotorPID.kI)},
                             {"RM kD", &(rightMotorPID.kD), 3, DOUBLE, sizeof(rightMotorPID.kD)},
                             {"LM kP", &(leftMotorPID.kP), 4, DOUBLE, sizeof(leftMotorPID.kP)},
                             {"LM kI", &(leftMotorPID.kI), 5, DOUBLE, sizeof(leftMotorPID.kI)},
                             {"LM kD", &(leftMotorPID.kD), 6, DOUBLE, sizeof(leftMotorPID.kD)},
                             {"Right Target Rps", &rightTargetFrequency, 7, INT, sizeof(rightTargetFrequency)},
                             {"Left Target Rps", &leftTargetFrequency, 8, INT, sizeof(leftTargetFrequency)}
                            };
Data dataReg[NUM_DATA] = {{"lPeriod", &lPeriod, 0, INT, sizeof(lPeriod)},
                          {"lFrequency", &lFrequency, 1, INT, sizeof(lFrequency)},
                          {"lPwm", &lPwm, 2, INT, sizeof(lPwm)},
                          {"rPeriod", &rPeriod, 3, INT, sizeof(rPeriod)},
                          {"rFrequency", &rFrequency, 4, INT, sizeof(rFrequency)},
                          {"rPwm", &rPwm, 5, INT, sizeof(rPwm)},
                          {"xAcc", &imuData[0], 6, INT, sizeof(imuData[0])},
                          {"yAcc", &imuData[1], 7, INT, sizeof(imuData[1])},
                          {"zAcc", &imuData[2], 8, INT, sizeof(imuData[2])},
                          {"xRot", &imuData[4], 9, INT, sizeof(imuData[4])},
                          {"yRot", &imuData[5], 10, INT, sizeof(imuData[5])},
                          {"zRot", &imuData[6], 11, INT, sizeof(imuData[6])}
                         };

void setup() {
  Serial.begin(9600);
  while (!Serial);

  botClient = new TCClient(ssid, password, ipAddress, paramReg, NUM_PARAMS, dataReg, NUM_DATA);
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
    timerAlarmWrite(controlLoopTimer, 10000, true); // 500 microseconds
    timerAlarmEnable(controlLoopTimer);
  interrupts();
  
  left.rotatePwm (60);
  right.rotatePwm (85);
}

void loop() {

  if (requestImuData) {
    imu.getAllData(imuData);
    requestImuData = 0;
  } else if (botClient->isConnectedToWifi()) {
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

  heartbeat ^= 0x01;
}
