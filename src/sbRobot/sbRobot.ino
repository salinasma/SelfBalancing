#include <MotorL.h>
#include <MotorR.h>
#include <Imu.h>
#include <controls.hpp>
#include <filters.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define MOTOR_CIRCUMFERENCE (double) 28.26

#define RIGHT_PWM1 26
#define RIGHT_PWM2 27
#define RIGHT_TACH1 18
#define RIGHT_TACH2 19
#define RIGHT_TACH_SAMPLE_INTERVAL 1000 // microseconds

#define LEFT_PWM1 25
#define LEFT_PWM2 33
#define LEFT_TACH1 34
#define LEFT_TACH2 35
#define LEFT_TACH_SAMPLE_INTERVAL 1000000 // microseconds

#define CUTOFF_FREQ 10  // Hz
#define TACH_SAMPLE_INTERVAL_SECONDS 0.001 // seconds
#define FILTER_ORDER  IIR::ORDER::OD1 // OD1 to OD4

#define INTERRUPT_PIN 2  // interrupt pin for IMU

/*-----IMU-----*/

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

Controls pitchPID (1, .09, .75, pi, -18, 17, -16, 16);

/*-----Motors-----*/

MotorL left (LEFT_PWM1, LEFT_PWM2, LEFT_TACH1, LEFT_TACH2, LEFT_TACH_SAMPLE_INTERVAL);
Controls leftMotorPID (1.2, .09, 0, pi, -17, 17, -220, 180);
//Filter leftFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);
Controls rightMotorPID (1, .09, 0, pi, -16, 16, -250, 250);
//Filter rightFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

int lPeriod = 0;
int lFrequency = 0;
int lPwm = 0;

int rPeriod = 0;
int rFrequency = 0;
int rPwm = 0;

int targetFreq = 50;
//int rightTargetFrequency = 50;
//int leftTargetFrequency = 50;

hw_timer_t* controlLoopTimer; 


void controlLoop(void *pvParameters) {
  uint8_t fb;


  left.rotatePwm (80);
//  right.rotatePwm (100);
    
  while (1) {
//    targetFreq = pitchPID.controlWorkLoop (ypr[2], 0);
//
//    Serial.println("Pitch: " + String(ypr[2]));
//    Serial.println("targetFreq: " + String(targetFreq));
    fb = (left.fbOutput[0] << 1) | left.fbOutput[1];
     
    lPeriod = left.getInstPeriodA();
    if (lPeriod != 0) { 
        lFrequency = 1000000/lPeriod;
    } else {
        lFrequency = 0;
    }

    // wheel is rotating backwards
    if (fb == 2) {
      lFrequency *= -1;
    }
    
//    lFrequency = leftFilter.filterIn((float)lFrequency);
    lPwm = leftMotorPID.controlWorkLoop (lFrequency, -4);

//    Serial.println("lFreq: " + String(lFrequency));
    Serial.println("lPwm: " + String(lPwm) + ", lFreq: " + String(lFrequency));

    fb = (right.fbOutput[0] << 1) | right.fbOutput[1];

    rPeriod = right.getInstPeriodA ();
    if (rPeriod != 0) {
        rFrequency = 1000000/rPeriod; // Hz or rotations-per-second
    } else {
        rFrequency = 0;
    }

    // wheel is rotating backwards
    if (fb == 1) {
      rFrequency *= -1;
    }
//    rFrequency = rightFilter.filterIn((float)rFrequency);
    rPwm = rightMotorPID.controlWorkLoop (rFrequency, -3);

//    Serial.println(", rPwm: " + String(rPwm)); // + ", rFreq: " + String(rFrequency));
    
    left.rotatePwm(lPwm);
//    right.rotatePwm(rPwm);

    delay(50);
  }
}
TaskHandle_t controlInitializer;

//void controlInit(void *pvParameters) {
//  controlLoopTimer = timerBegin(0, 80, true); // each timer tick is 1 us                                                             
//  timerAttachInterrupt(controlLoopTimer, controlLoop, true);
//  timerAlarmWrite(controlLoopTimer, 1000, true); // 500 microseconds
//  timerAlarmEnable(controlLoopTimer);
//
//  int count = 0;
//
//  while (1) {
//    count++;
//  }
//}

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

TaskHandle_t imuReader;

void imuReadLoop(void *pvParameters) { 
    // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3V or Arduino
  // Pro Mini running at 3.3V, cannot handle this baud rate reliably due to
  // the baud timing being too misaligned with processor ticks. You must use
  // 38400 or slower in these cases, or use some kind of external separate
  // crystal solution for the UART timer.

  // initialize device
//  Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  // verify connection
//  Serial.println(F("Testing device connections..."));
//  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

  // load and configure the DMP
//  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(-368);
  mpu.setYGyroOffset(116);
  mpu.setZGyroOffset(24);
  mpu.setZAccelOffset(709); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready
//    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
//    Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
//    Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
//    Serial.println(F(")..."));
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
//    Serial.print(F("DMP Initialization failed (code "));
//    Serial.print(devStatus);
//    Serial.println(F(")"));
  }
  
  while (1) {
    // if programming failed, don't try to do anything
  if (!dmpReady) return;

  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    if (mpuInterrupt && fifoCount < packetSize) {
        // try to get out of the infinite loop 
        fifoCount = mpu.getFIFOCount();
    }  
      // other program behavior stuff here
      // .
      // .
      // .
      // if you are really paranoid you can frequently test in between other
      // stuff to see if mpuInterrupt is true, and if so, "break;" from the
      // while() loop to immediately process the MPU data
      // .
      // .
      // .

      uint8_t fb = (right.fbOutput[1] << 1) | right.fbOutput[0]; 

//      if (fb == 2) {
//        Serial.println ("Forward");
//      } else if (fb == 1) {
//        Serial.println ("Backward");
//      } else if (fb == 3) {
//        Serial.println ("Stationary");
//      } else {
//        Serial.println ("Unexpected result " + String(fb));
//      }
      
  }

  // reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // get current FIFO count
  fifoCount = mpu.getFIFOCount();
  if(fifoCount < packetSize){
    //Lets go back and wait for another interrupt. We shouldn't be here, we got an interrupt from another event
    // This is blocking so don't do it while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
  }
  // check for overflow (this should never happen unless our code is too inefficient)
  else if ((mpuIntStatus & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT)) || fifoCount >= 1024) {
    // reset so we can continue cleanly
    mpu.resetFIFO();
    //  fifoCount = mpu.getFIFOCount();  // will be zero after reset no need to ask
    Serial.println(F("FIFO overflow!"));

  // otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {

        // read a packet from FIFO
    while(fifoCount >= packetSize){ // Lets catch up to NOW, someone is using the dreaded delay()!
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      // track FIFO count here in case there is > 1 packet available
      // (this lets us immediately read more without waiting for an interrupt)
      fifoCount -= packetSize;
    }
            // display yaw, pitch, and roll in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
//            Serial.print("ypr\t");
//            Serial.print(ypr[0] * 180/M_PI);
//            Serial.print("\t");
//            Serial.print(ypr[1] * 180/M_PI);
//            Serial.print("\t");
//            Serial.println(ypr[2] * 180/M_PI);
  }
  }
}

void setup() {
  // initialize serial communication
  // really up to you depending on your project)
  Serial.begin(9600);
  while (!Serial); // wait for Leonardo enumeration, others continue immediately

//  xTaskCreatePinnedToCore(
//                    imuReadLoop,   /* Task function. */
//                    "IMU Read",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    1,           /* priority of the task */
//                    &imuReader,      /* Task handle to keep track of created task */
//                    0);          /* pin task to core 0 */                  
//  delay(500);

  xTaskCreatePinnedToCore(
                    controlLoop,   /* Task function. */
                    "Robot Balance",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &controlInitializer,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 0 */                  
  delay(500);
}

void loop() {

}
