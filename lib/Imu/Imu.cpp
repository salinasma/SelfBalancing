/*
 * Author: Pramuka Perera
 * Description: Software interface for HiLetGo GY-251 IMU
 */

#include "Imu.h"

Imu::Imu (int interruptPin) {
    intPin = interruptPin;
    pinMode (intPin, INPUT_PULLUP);

    Wire.begin ();    
    writeByte (MPU6050_ADDRESS, PWR_MGMT_1_REG, 0x00);      // Wake up imu
//    writeByte( MPU6050_ADDRESS, SIGNAL_PATH_RESET, 0x07);   // Reset all internal signal paths in the MPU-6050 by writing 0x07 to register 0x68;
//    writeByte( MPU6050_ADDRESS, I2C_SLV0_ADDR, 0x20);       // Write register 0x37 to select how to use the interrupt pin. For an active high, push-pull signal that stays until register (decimal) 58 is read, write 0x20.
//    writeByte( MPU6050_ADDRESS, ACCEL_CONFIG, 0x01);        // Write register 28 (==0x1C) to set the Digital High Pass Filter, bits 3:0. For example set it to 0x01 for 5Hz. (These 3 bits are grey in the data sheet, but they are used! Leaving them 0 means the filter always outputs 0.)
//    writeByte( MPU6050_ADDRESS, MOT_THR, 20);               // Write the desired Motion threshold to register 0x1F (For example, write decimal 20).  
//    writeByte( MPU6050_ADDRESS, MOT_DUR, 40 );              // Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate  
//    writeByte( MPU6050_ADDRESS, MOT_DETECT_CTRL, 0x15);     // To register 0x69, write the motion detection decrement and a few other settings (for example write 0x15 to set both free-fall and motion decrements to 1 and accelerometer start-up delay to 5ms total by adding 1ms. )   
//    writeByte( MPU6050_ADDRESS, INT_ENABLE, 0x40 );         // Write register 0x38, bit 6 (0x40), to enable motion detection interrupt.     
//    writeByte( MPU6050_ADDRESS, 0x37, 160 );                // Now INT pin is active low
}

void Imu::writeByte (uint8_t address, uint8_t subAddress, uint8_t data) {
    Wire.beginTransmission (address);
    Wire.write (subAddress);
    Wire.write (data);
    Wire.endTransmission ();
}

int Imu::getXDisplacement (void) { 
    Wire.beginTransmission (MPU6050_ADDRESS);
    Wire.write (0x43);
    Wire.endTransmission (false);
    Wire.requestFrom ((uint16_t)MPU6050_ADDRESS, (uint8_t)2, true);
    return (Wire.read () << 8 | Wire.read ());
}

int Imu::getYDisplacement (void) { 
    Wire.beginTransmission (MPU6050_ADDRESS);
    Wire.write (0x45);
    Wire.endTransmission (false);
    Wire.requestFrom ((uint16_t)MPU6050_ADDRESS, (uint8_t)2, true);
    return (Wire.read () << 8 | Wire.read ());
}

int Imu::getZDisplacement (void) { 
    Wire.beginTransmission (MPU6050_ADDRESS);
    Wire.write (0x47);
    Wire.endTransmission (false);
    Wire.requestFrom ((uint16_t)MPU6050_ADDRESS, (uint8_t)2, true);
    return (Wire.read () << 8 | Wire.read ());
}

int Imu::getXAcceleration (void) { 
    Wire.beginTransmission (MPU6050_ADDRESS);
    Wire.write (0x3B);
    Wire.endTransmission (false);
    Wire.requestFrom ((uint16_t)MPU6050_ADDRESS, (uint8_t)2, true);
    return (Wire.read () << 8 | Wire.read ());
}

int Imu::getYAcceleration (void) { 
    Wire.beginTransmission (MPU6050_ADDRESS);
    Wire.write (0x3D);
    Wire.endTransmission (false);
    Wire.requestFrom ((uint16_t)MPU6050_ADDRESS, (uint8_t)2, true);
    return (Wire.read () << 8 | Wire.read ());
}

int Imu::getZAcceleration (void) { 
    Wire.beginTransmission (MPU6050_ADDRESS);
    Wire.write (0x3F);
    Wire.endTransmission (false);
    Wire.requestFrom ((uint16_t)MPU6050_ADDRESS, (uint8_t)2, true);
    return (Wire.read () << 8 | Wire.read ());
}

