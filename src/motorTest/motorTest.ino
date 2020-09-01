#include <MotorL.h>
#include <MotorR.h>

#define NUM_SAMPLES 4 // must be a power of 2

MotorL left(26, 27, 18, 19);
MotorR right(25, 33, 34, 35);

void setup() {
  Serial.begin (9600);
  while (!Serial) {
    ;
  }
  Serial.println ("Motor Test");

  left.rotate (-100);
  right.rotate (-100);
}

int period;
int periodHistory [NUM_SAMPLES] = {0};
int periodIndex = 0;
int avgPeriod = 0;

void loop() {
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
  
  delay(1000);
}
