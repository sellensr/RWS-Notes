#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()
unsigned timeLast = 0;  // the last time we went through the loop, microseconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  while(!Serial && millis() < 5000); // wait for the USB to wake up
  Serial.printf("\n\nRick's S1.2\n\n"); // worth printing out the name of the sketch to document
  Serial.printf("Time [s], Pin 12, A0, Delta t [us], float error [us], double error [us]\n"); // provide headings to name the outputs
  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  analogReadResolution(12);
}

void loop() {
  // Keep time in microseconds for best accuracy.
  unsigned timeNow = micros();      // the time we started this loop, microseconds
  // 32 bit unsigned can hold 0,1,2,...,4294967295, about 4 billion, a little more than 71 minutes of us
  float timeNowS_float = timeNow / 1000000.;   // time now in seconds, float so it will have decimals
  // 32 bit floats have 6 or 7 significant figures and can reach +/- 3.4E38, so timeNowS_float is less precise 
  double timeNowS = timeNow / 1000000.;   // time now in seconds, double so it will have more decimals
  // 64 bit doubles have about 16 significant figures and can reach +/- 1E308, so timeNowS is accurate 
  unsigned dt = timeNow - timeLast; // the time difference since last loop, microseconds
  // note that addition and subtraction wrap around for unsigned variables, so dt always works
  // 4294967295 + 1 = 0, 4294967295 + 2 = 1,...  0 - 4294967295 = 1, 3 - 4294967295 = 4, ...

  // get input and calculate values
  int d = digitalRead(12);            // 16 bit integer can hold -32768,...,-2,-1,0,1,2,..., 32767
  unsigned a = analogRead(A0);        // 16 bit unsigned can hold whole numbers 0,1,2,...,65535 
  byte led = digitalRead(13);         // 8 bit byte can hold whole numbers 0,1,2,...,255
  
  if(led == 1) digitalWrite(13,0);
  else digitalWrite(13,1);

  // Print out current status, to 3 places usually precise enough
  Serial.printf("%8.3f, %d, %5u, %8u, %8.5f, %8.5f\n", timeNowS, d, a, dt,
                timeNowS_float * 1000000 - timeNow, // error in float value of time [us] 
                timeNowS * 1000000 - timeNow);      // error in double value of time [us]           
//  Serial.printf("%8.3f, %d, %u, %u\n", timeNowS, d, a, dt);           

  timeLast = timeNow;      // save the old value for next time through the loop

  // The only time to use delay is to slow down a sketch so you can see what is going on
  delay(500);   // delay() is bad -- drone crashes happen in split seconds!
}
