#include <MotorL.h>
#include <MotorR.h>

#define NUM_SAMPLES 4 // must be a power of 2



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
MotorR right (RIGHT_PWM1, RIGHT_PWM2, RIGHT_TACH1, RIGHT_TACH2, RIGHT_TACH_SAMPLE_INTERVAL);
// Controls leftMotorPID (4, .09, .75, pid, 0, 1470);
// Filter leftFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);

// Controls rightMotorPID (4, .09, .75, pid, 0, 1470);
// Filter rightFilter(CUTOFF_FREQ, TACH_SAMPLE_INTERVAL_SECONDS, FILTER_ORDER);





extern float timeStep;


// MotorL left(26, 27, 18, 19);
// MotorR right(25, 33, 34, 35);

void setup() {
  Serial.begin (9600);
  while (!Serial) {
    ;
  }
  Serial.println ("Motor Test");

  left.rotate (-100);
  //right.rotate (-100);
}

int period;
int periodHistory [NUM_SAMPLES] = {0};
int periodIndex = 0;
int avgPeriod = 0;

void loop() {
  // period = left.getRotationPeriodA ();
  // periodHistory[periodIndex] = period;
  // periodIndex = (periodIndex + 1) % NUM_SAMPLES;

  // avgPeriod = 0;
  // for (int i = 0; i < NUM_SAMPLES; i++) {
  //   avgPeriod += periodHistory[i];
  // }
  // avgPeriod /= NUM_SAMPLES;

  // Serial.println ("Period: " + String (period));
  // Serial.println ("Average Period: " + String (avgPeriod));

  // if ( period != 0) {
  //   Serial.println ("Frequency: " + String (1000000/period));
  // }
  // Serial.println ("-----------------");

  delay(100);

  Serial.print ("Position: ");
  Serial.print ( left.getMotorState().position);
//  Serial.print (", Velocity: ");
//  Serial.print ( left.getMotorState().velocity);
  Serial.print (", Acceleration: ");
  Serial.print ( left.getMotorState().acceleration);
  Serial.print (", timeStep: ");
  Serial.print ( timeStep);
//  Serial.print (", one: ");
//  Serial.print ( left.getMotorState().one);
//  Serial.print (", two: ");
//  Serial.print ( left.getMotorState().two);
//  Serial.print (", three: ");
//  Serial.print ( left.getMotorState().three);

//    Serial.print (", A_count: ");
//  Serial.print ( left.getMotorState().a_count);
//  Serial.print (", B_count: ");
//  Serial.print ( left.getMotorState().b_count);

//  Serial.print (", forward: ");
//  Serial.print ( left.getMotorState().fw);
//  Serial.print (", backwards: ");
//  Serial.print ( left.getMotorState().bw);
  
//  Serial.print (", input: ");
//  Serial.print ( left.getMotorState().input);
//  Serial.print (", output: ");
//  Serial.print ( left.getMotorState().output);
//  Serial.print (", State: ");
//  Serial.print ( left.getMotorState().placeInFSM);
//  Serial.print (", InvalidCount: ");
//  Serial.print ( left.getMotorState().invalidCount);
    Serial.println ( );

}
