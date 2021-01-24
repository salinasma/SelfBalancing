//
//int newStringAvailable = 0;
//String str = "";
//
//void setup() {
//  str.reserve (200);
//  
//  Serial.begin (9600);
//  Serial.setTimeout (10);
//}
//
//void loop() {
//  while (!newStringAvailable);
//
//  Serial.println (str);
//  newStringAvailable = 0;
//}
//
//void serialEvent () {
//  str = Serial.readString ();
//  newStringAvailable = 1;
//}

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  Serial.println ("Serial Event Test\n");
}

void loop() {
  Serial.println ("Serial Event Test\n");
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
