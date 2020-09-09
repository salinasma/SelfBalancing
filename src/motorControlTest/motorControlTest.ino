#include <MotorL.h>
#include <MotorR.h>
#include <Imu.h>
#include <controls.hpp>

#define LEFT_PWM1 26
#define LEFT_PWM2 27
#define LEFT_TACH1 18
#define LEFT_TACH2 19
#define LEFT_TACH_SAMPLE_INTERVAL 1000

#define RIGHT_PWM1 25
#define RIGHT_PWM2 33
#define RIGHT_TACH1 34
#define RIGHT_TACH2 35
#define RIGHT_TACH_SAMPLE_INTERVAL 1000

#define NUM_SAMPLES 128 // must be a power of 2

MotorL left (LEFT_PWM1, LEFT_PWM2, LEFT_TACH1, LEFT_TACH2, LEFT_TACH_SAMPLE_INTERVAL);
MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);
Imu imu (32);

Controls leftMotorPID (4, .1, 1, pid, 0, 1470);
Controls rightMotorPID (4, .1, 1, pid, 0, 1470);

//int periodHistory [NUM_SAMPLES] = {0};
//int periodIndex = 0;
//int avgPeriod = 0;

int lPeriod;
int lFrequency = 0;
int lPwm = 0;

int rPeriod;
int rFrequency = 0;
int rPwm = 0;

void setup() {
    Serial.begin (9600);
    while (!Serial) {;}
    Serial.println ("Motor Control Test");

    left.rotate (70);
    right.rotate (70);
}


void loop() {
//    periodHistory[periodIndex] = lPeriod;
//    periodIndex = (periodIndex + 1) % NUM_SAMPLES;
//
//    avgPeriod = 0;
//    for (int i = 0; i < NUM_SAMPLES; i++) {
//        avgPeriod += periodHistory[i];
//    }
//    avgPeriod /= NUM_SAMPLES;

    lPeriod = left.getAvgPeriodA();
    if (lPeriod != 0) { 
        lFrequency = 1000000/lPeriod;
    } else {
        lFrequency = 0;
    }
    lPwm = leftMotorPID.controlWorkLoop (lFrequency, 100);

    rPeriod = right.getAvgPeriodA ();
    if (rPeriod != 0) {
        rFrequency = 1000000/rPeriod;
    } else {
        rFrequency = 0;
    }
    rPwm = rightMotorPID.controlWorkLoop (rFrequency, 100);

    Serial.println ("Right_Freq: " + String (rFrequency));
    Serial.print (" ");
//    Serial.println ("Left_Freq: " + String (lFrequency));
//    Serial.print (" ");
//    Serial.println ("Left_PWM: " + String (lPwm));
//    Serial.print (" ");
//    Serial.println ("Right_PWM: " + String (rPwm));
//    Serial.print (" ");
//    Serial.println ("-----------------");
    
    left.rotate (lPwm);
    right.rotate (rPwm);
}
