#include "RTClib.h"

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = micros();      // the time we started this loop, microseconds
unsigned long timeLast = 0;            // the last time we went through the loop, microseconds
unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.printf("\n\nRick's S1.4\n\n"); // worth printing out the name of the sketch to document
  Serial.printf("Time [us], Pin 12, A3, dt, w, smoothed A3\n"); // provide headings to name the outputs
  pinMode(12,INPUT_PULLUP); // Pushbutton input
  pinMode(10,OUTPUT);       // Set 10 to ground to pull PB low
  digitalWrite(10,LOW);
  pinMode(13,OUTPUT);       // the LED
  analogReadResolution(16);
}

void loop() {
  timeLast = timeNow;      // save the old value before getting the new one
  timeNow = micros();      // the time we started this loop, microseconds
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  
  float timeNowS = timeNow / 1000000.;   // time now in seconds, float so it will have decimals

  // put a slowly changing square wave on A0 for testing (connect A0 and A3 to read it)
  if(timeNow % 2000000 < 1000000) analogWrite(A0,1000); else analogWrite(A0,100);

  // get input and calculate values
  int d = digitalRead(12);            // 16 bit integer can hold -32768,...,-2,-1,0,1,2,..., 32767
  unsigned a = analogRead(A3);        // 16 bit unsigned can hold whole numbers 0,1,2,...,65535 
  byte led = digitalRead(13);         // 8 bit byte can hold whole numbers 0,1,2,...,255
  unsigned long tau = 100000;         // the smoothing time constant in microseconds bigger = smoother, slower
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g. 3333/10000 = 0
  float w = (float) dt/tau;           // weight to give the latest reading of a in smoothing
  static float as = 0;                // exponentially smoothed version of a, static so it persists
  if(w < 1.0) 
    as = w * a + (1-w) * as;          // weighted average will smooth the data to reduce noise
  else as = a;                        // use the latest value if it has been a long time, dt > tau

  static unsigned long lastPrint = 0;// Print out current status, but not every time
  // UNO: this printing will take over 3 ms, but the rest of the loop only takes about 240 us
  // M0: this printing will take almost 1 ms, but the rest of the loop only takes about 80 us
  if((timeNow-lastPrint)/1000000. >= 0.05){     // test if time to print in seconds
    lastPrint = timeNow;
    Serial.printf("%8.3f, %5u, %9.3f\n", timeNowS, a, as);
  }

  timeLast = timeNow;      // save the old value for next time through the loop

  // The only time to use delay is to slow down a sketch so you can see what is going on
  //delay(500);   // delay() is bad -- drone crashes happen in split seconds!
}
