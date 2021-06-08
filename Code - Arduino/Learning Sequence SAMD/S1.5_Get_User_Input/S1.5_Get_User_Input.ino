#include "RTClib.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.printf("\n\nRick's S1.5\n\n"); // worth printing out the name of the sketch to document
}

void loop() {
  Serial.setTimeout(15000);        // Wait this many milliseconds before moving on
  Serial.printf("\nEnter a String in the box at the top and hit return\n"); // Read all the characters you can until <Return>
  String scratch = Serial.readStringUntil('\n');    // or timeout...
  // Two different ways to deal with the Arduino String type
  Serial.printf("    scratch.c_str() points to: %s\n", scratch.c_str()); 
  Serial.printf("Serial.println(scratch) gives: "); 
  Serial.println(scratch); 
  
  Serial.printf("\nEnter an Integer in the box at the top and hit return\n");
  int i = Serial.parseInt();      // Keep trying to get an int until timeout
  Serial.printf("i = %d", i);                      // print the results
  
  Serial.printf("\nEnter a Float in the box at the top and hit return\n");           
  float x = Serial.parseFloat();  // Keep trying to get a float until timeout
  Serial.printf("x = %f", x);                      // print the results

  Serial.printf("\nEnter some characters in the box at the top and hit return\n");
  while(!Serial.available());     // Wait until there are characters
  while(Serial.available()){      // read them until they are gone
    char c = Serial.read();       // read a character
    Serial.printf("c = %c\n", c);                    // print the results
  }

}
