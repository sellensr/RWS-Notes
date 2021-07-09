// This blank Starter has all of the basics covered so you can focus on your code
#include "globals.h" 

void setup() {
  // put your setup code here, to run once:
  setup217Kit();
  Serial.print("\n\nMECH 217 Blank Starter\n\n");
}

void loop() {
  // Update the global variables for the time in this loop
  timeLast = timeNow;      // the last time we went through the loop, microseconds
  timeNow = micros();      // the time we started this loop, microseconds
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  
  // put your main code here, to run repeatedly:

  
  delay(20);  // delay only to slow things down so you can see
}
