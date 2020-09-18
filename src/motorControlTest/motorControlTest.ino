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

MotorL left (LEFT_PWM1, LEFT_PWM2, LEFT_TACH1, LEFT_TACH2, LEFT_TACH_SAMPLE_INTERVAL);
Controls leftMotorPID (4, .09, .75, pid, 0, 1470);
Filter leftFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);
Controls rightMotorPID (4, .09, .75, pid, 0, 1470);
Filter rightFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

Imu imu (32);

int lPeriod;
int lFrequency = 0;
int lPwm = 0;

int rPeriod;
int rFrequency = 0;
int rPwm = 0;

void setup() {
    Serial.begin (9600);
    while (!Serial) {;}
//    Serial.println ("Motor Control Test");

    left.rotate (70);
    right.rotate (70);
}

int logIndex = 0;
void loop() {
    lPeriod = left.getAvgPeriodA();
    if (lPeriod != 0) { 
        lFrequency = 1000000/lPeriod;
    } else {
        lFrequency = 0;
    }
    lFrequency = leftFilter.filterIn((float)lFrequency);
    lPwm = leftMotorPID.controlWorkLoop (lFrequency, 150);

    rPeriod = right.getAvgPeriodA ();
    if (rPeriod != 0) {
        rFrequency = 1000000/rPeriod; // Hz or rotations-per-second
    } else {
        rFrequency = 0;
    }
    rFrequency = rightFilter.filterIn((float)rFrequency);
    rPwm = rightMotorPID.controlWorkLoop (rFrequency, 150);


// Print uniformly sampled tach data (sample period is 1 ms)
// Collects a total of 1000 samples
//    Serial.println (String (logIndex) + ", " + String (left.tachLogger[logIndex]));
//    if (logIndex < left.tachLoggerIndex) { 
//      logIndex++;
//    }

//    Serial.print ("Right_Freq: " + String (rFrequency));
//    Serial.print (",");
//    Serial.println ("Right_PWM: " + String (rPwm));

    Serial.print ("Left_Freq: " + String (lFrequency));
    Serial.print (",");
    Serial.println ("Left_PWM: " + String (lPwm));


    
   left.rotate (lPwm);
   right.rotate (rPwm);
}
