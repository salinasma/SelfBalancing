#include <MotorL.h>
#include <MotorR.h>

#define RIGHT_PWM1 26
#define RIGHT_PWM2 27
#define RIGHT_TACH1 18
#define RIGHT_TACH2 19

#define LEFT_PWM1 25
#define LEFT_PWM2 33
#define LEFT_TACH1 34
#define LEFT_TACH2 35

MotorL left ( LEFT_PWM1, LEFT_PWM2, 
              LEFT_TACH1, LEFT_TACH2, 
              -1);


MotorR right (  RIGHT_PWM1, RIGHT_PWM2, 
                RIGHT_TACH1, RIGHT_TACH2, 
                -1);

void setup(){
  Serial.begin(9600);
  while (!Serial);  
}

void loop() {
  Serial.println("Start");
  left.stopMotor = 0;
  left.rotateDegrees(720);
  left.rotatePwm(70);

  right.stopMotor = 0;
  right.rotateDegrees(720);
  right.rotatePwm(85);

  while (left.stopMotor == 0 || right.stopMotor == 0) {
      if (left.stopMotor) {
        left.rotatePwm(10);
      }

      if (right.stopMotor) {
        right.rotatePwm(10);
      }
  }

  left.rotatePwm(10);
  right.rotatePwm(10);
  
  delay(1000);
  Serial.println("End");
}
