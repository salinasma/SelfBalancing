#include "controls.hpp"

// Motor A
int leftMotor = 22;
int motor1Pin2 = 27;

int directionPin = 23;

// Setting PWM properties
const int freq = 2000;
const int pwmChannel = 1;
const int leftPWMChannel = 0;

const int leftMotor_A = 0;
const int leftMotorPWMChannel_A = 0;

const int leftMotor_B = 0;
const int leftMotorPWMChannel_B = 0;

const int rightMotor_A = 0;
const int rightMotorPWMChannel_A = 0;

const int rightMotor_B = 0;
const int rightMotorPWMChannel_B = 0;
const int resolution = 8;
int dutyCycle = 1000;

Controls *leftMotor;
Controls *rightMotor;
Controls *IMUMotor;



void initEncoders() {
  // to do edge triggered interrupts
  // int interruptPin 5
  // attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

  // to do edge triggered interrupts
  // int interruptPin 5
  // attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

  // to do edge triggered interrupts
  // int interruptPin 5
  // attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);

  // to do edge triggered interrupts
  // int interruptPin 5
  // attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
}

void initDrivers() {
  pinMode(leftMotor_A, OUTPUT);
  ledcSetup(leftMotorPWMChannel_A, freq, resolution);
  digitalWrite(leftMotor_A, HIGH);
  ledcAttachPin(leftMotor_A, leftMotorPWMChannel_A);

  pinMode(leftMotor_B, OUTPUT);
  ledcSetup(leftMotorPWMChannel_B, freq, resolution);
  digitalWrite(leftMotor_B, HIGH);
  ledcAttachPin(leftMotor_B, leftMotorPWMChannel_B);

  pinMode(rightMotor_A, OUTPUT);
  ledcSetup(leftMotorPWMChannel_A, freq, resolution);
  digitalWrite(rightMotor_A, HIGH);
  ledcAttachPin(rightMotor_A, leftMotorPWMChannel_A);

  pinMode(rightMotor_B, OUTPUT);
  ledcSetup(leftMotorPWMChannel_B, freq, resolution);
  digitalWrite(rightMotor_B, HIGH);
  ledcAttachPin(rightMotor_B, leftMotorPWMChannel_B);
}

void initHeartBeat() {

  pinMode(LED_BUILTIN, OUTPUT);
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(LED_BUILTIN, pwmChannel);
}

void initControls() {
  leftMotor = new Controls(1, 1, 1 ControlType.PID, -100, 100);
  rightMotor = new Controls(1, 1, 1 ControlType.PID, -100, 100);
  IMUMotor = new Controls(1, 1, 1 ControlType.PID, -100, 100);
}

void setup() {
  // initialize control over the keyboard:
  //  Keyboard.begin();
  // initialize digital pin LED_BUILTIN as an output.
  initHeartBeat();
  initDrivers();
  initEncoders();
  initControls();
  pinMode(directionPin, OUTPUT);

  digitalWrite(directionPin, HIGH);

  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
  Serial.print(dutyCycle);

  //  ledcWrite(leftPWMChannel, dutyCycle);
}

void loop() {
  // left Motor
  leftMotor->controlWorkLoop();

  // right Motor
  rightMotor->controlWorkLoop();
  IMUMotor->controlWorkLoop();
}
