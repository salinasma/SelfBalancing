#include <MotorL.h>
#include <MotorR.h>
#include <Imu.h>

#define NUM_SAMPLES 4 // must be a power of 2

char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

MotorL left(26, 27, 18, 19);
MotorR right(25, 33, 34, 35);
Imu imu(32);


void setup() {
  Serial.begin (9600);
  while (!Serial) {
    ;
  }
  Serial.println ("Motor-IMU Integration Test");

  left.rotate (100);
  right.rotate (100);
}

int period;
int periodHistory [NUM_SAMPLES] = {0};
int periodIndex = 0;
int avgPeriod = 0;

void loop() { 
  // print out data
  Serial.print("aX = "); Serial.print(convert_int16_to_str(imu.getXAcceleration()));
  Serial.print(" | aY = "); Serial.print(convert_int16_to_str(imu.getYAcceleration()));
  Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(imu.getZAcceleration()));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  // Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
  Serial.print(" | gX = "); Serial.print(convert_int16_to_str(imu.getXDisplacement()));
  Serial.print(" | gY = "); Serial.print(convert_int16_to_str(imu.getYDisplacement()));
  Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(imu.getZDisplacement()));
  Serial.println();

  Serial.println ("-----------------");

  period = left.getRotationPeriodA ();
  periodHistory[periodIndex] = period;
  periodIndex = (periodIndex + 1) % NUM_SAMPLES;

  avgPeriod = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    avgPeriod += periodHistory[i];
  }
  avgPeriod /= NUM_SAMPLES;
  
  Serial.println ("Period: " + String (period));
  Serial.println ("Average Period: " + String (avgPeriod));

  if ( period != 0) {
    Serial.println ("Frequency: " + String (1000000/period));
  }
  Serial.println ("-----------------");
  
  // delay
  delay(1000);
}
