#include <Imu.h>

#define IMU_INT_PIN 4

Imu imu(IMU_INT_PIN);

int imuData[7] = {-1, -1, -1, -1, -1, -1, -1};
int dataAvail = 0;

void imuHandler() {
//  xAcc = imu.getXAcceleration();
//  yAcc = imu.getYAcceleration();
//  zAcc = imu.getZAcceleration();
//  
//  xRot = imu.getXRotation();
//  yRot = imu.getYRotation();
//  zRot = imu.getZRotation();
//  imu.getAllData(imuData);
  dataAvail = 1;
  
//  imu.ackInterrupt();
  //Imu::writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x00);  // Disable IMU INT pin interrupt
}

hw_timer_t* timer; 

void enableImuInterrupt() {
    Imu::writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x32); // Enable IMU INT pin interrupt
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  noInterrupts();
    timer = timerBegin(0, 80, true); // each timer tick is 1 microsecond                                                             
    timerAttachInterrupt(timer, imuHandler, true);
    timerAlarmWrite(timer, 10, true); // 10 milliseconds
    timerAlarmEnable(timer);
//    attachInterrupt(digitalPinToInterrupt(IMU_INT_PIN), imuHandler, LOW);
  interrupts();
  
//    Imu::writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x32); // Enable IMU INT pin interrupt
//    Serial.println("Int Enable: " + String(Imu::readByte(INT_ENABLE)));
}

void loop() {
  if (dataAvail) {
//  xAcc = imu.getXAcceleration();
//  yAcc = imu.getYAcceleration();
//  zAcc = imu.getZAcceleration();
//  
//  xRot = imu.getXRotation();
//  yRot = imu.getYRotation();
//  zRot = imu.getZRotation();
    Serial.println("Time: " + String(micros()));
    Serial.print("xAcc: " + String(imuData[0]));
    Serial.print(", yAcc: " + String(imuData[1]));
    Serial.print(", zAcc: " + String(imuData[2]));
    Serial.print(", xRot: " + String(imuData[4]));
    Serial.print(", yRot: " + String(imuData[5]));
    Serial.println(", zRot: " + String(imuData[6]));
    dataAvail = 0;
  }
  else {
    Serial.println("No new data");
  }
  
  delay(1000);
}
