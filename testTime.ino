/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Motor A
int leftMotor = 22; 
int motor1Pin2 = 27; 
 
int directionPin = 23;
 
// Setting PWM properties
const int freq = 2000;
const int pwmChannel = 1;
const int leftPWMChannel = 0;
const int resolution = 8;
int dutyCycle = 1000;

void setup() {
  // initialize control over the keyboard:
//  Keyboard.begin();
    // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(leftMotor, OUTPUT);

  digitalWrite(directionPin, HIGH);   
   digitalWrite(leftMotor, HIGH);   
  
  
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(leftPWMChannel, freq, resolution);
  
  ledcAttachPin(leftMotor, leftPWMChannel);
  ledcAttachPin(LED_BUILTIN,pwmChannel );
  Serial.begin(115200);

  // testing
  Serial.print("Testing DC Motor...");
  Serial.print (dutyCycle);
  
//  ledcWrite(leftPWMChannel, dutyCycle);  
}


void loop() {



    
  
  if  ( dutyCycle >= 128 ) {
    dutyCycle = 72;
    } 
  else 
    dutyCycle +=1;

      Serial.print (dutyCycle);
  Serial.print ("\n");
  ledcWrite(pwmChannel, dutyCycle); 
    ledcWrite(leftPWMChannel, dutyCycle);  
    delay(50);
    
}
