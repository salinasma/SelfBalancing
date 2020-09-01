/*
 * Author: Pramuka Perera
 * Description: Software interface for HiLetGo GY-251 IMU
 */

#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>

#define MPU6050_ADDRESS    (uint8_t)0x68
#define SIGNAL_PATH_RESET  (uint8_t)0x68
#define PWR_MGMT_1_REG     (uint8_t)0x6B
#define I2C_SLV0_ADDR      (uint8_t)0x37
#define ACCEL_CONFIG       (uint8_t)0x1C 
#define MOT_THR            (uint8_t)0x1F  // Motion detection threshold bits [7:0]
#define MOT_DUR            (uint8_t)0x20  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define MOT_DETECT_CTRL    (uint8_t)0x69
#define INT_ENABLE         (uint8_t)0x38
#define WHO_AM_I_MPU6050   (uint8_t)0x75 // Should return 0x68
#define INT_STATUS         (uint8_t)0x3A

class Imu {
    public:
        Imu (int interruptPin);

        int getXDisplacement (void);
        int getYDisplacement (void);
        int getZDisplacement (void);

        int getXAcceleration (void);
        int getYAcceleration (void);
        int getZAcceleration (void);
        
    private:
        static void writeByte (uint8_t address, uint8_t subAddress, uint8_t data);

//        int gyroX, gyroY, gyroZ;
//        int accelX, accelY, accelZ;
        int intPin;
};

#endif
