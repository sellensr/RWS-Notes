#include "S.h"    // We can hide some of the clutter in another tab

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  P("\n\nRick's S1.4\n\n"); // worth printing out the name of the sketch to document
  P("Time [us], Pin 10, A0, dt, w, smoothed A0\n"); // provide headings to name the outputs
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();      // the time we started this loop, microseconds
  // unsigned long can hold 0,1,2,...,4294967295, about 4 billion, a little more than 71 minutes of us
  float timeNowS = timeNow / 1000000.;   // time now in seconds, float so it will have decimals
  // 32 bit floats have 6 or 7 significant figures and can reach +/- 3.4E38, so timeNowS is less precise 
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds

  // get input and calculate values
  int d = digitalRead(10);            // 16 bit integer can hold -32768,...,-2,-1,0,1,2,..., 32767
  unsigned a = analogRead(A0);        // 16 bit unsigned can hold whole numbers 0,1,2,...,65535 
  byte led = digitalRead(13);         // 8 bit byte can hold whole numbers 0,1,2,...,255
  unsigned long tau = 100000;         // the smoothing time constant in microseconds bigger = smoother, slower
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g. 3333/10000 = 0
  float w = min(1.,(float) dt/tau);   // weight to give the latest reading of a in smoothing
  static float as = 0;                // exponentially smoothed version of a, static so it persists
  as = w * a + (1-w) * as;            // weighted average will smooth the data to reduce noise
  
  if(led == 1) digitalWrite(13,0);    // act on the results
  else digitalWrite(13,1);

  static unsigned long lastPrint = 0;// Print out current status, but not every time
  // this printing will take over 3 ms, but the rest of the loop only takes about 240 us
  if((timeNow-lastPrint)/1000000. >= 0.1){     // test if time to print in seconds
    lastPrint = timeNow;
    P(timeNowS,3);       // time to 3 places usually precise enough
    PCS(d);
    PCS(a);
    PCS(dt);            // printing everything will help you debug
    PCS(w,5);           // can always comment out later
    PCSL(as,5);
  }

  timeLast = timeNow;      // save the old value for next time through the loop

  // The only time to use delay is to slow down a sketch so you can see what is going on
  //delay(500);   // delay() is bad -- drone crashes happen in split seconds!
}
