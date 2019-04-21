#include "S.h"    // We can hide some of the clutter in another tab
// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds
float timeLastS = 0;         // a floating point version

void setup() {
  Serial.begin(115200);     // set the serial port speed
}

void loop() {
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();          // the time we started this loop, microseconds
  // unsigned long can hold 0,1,2,...,4294967295, about 4 billion, a little more than 71 minutes of us
  float timeNowS = timeNow / 1000000.;       // time now in seconds, float so it will have decimals
  // 32 bit floats have 6 or 7 significant figures and can reach +/- 3.4E38, so timeNowS is less precise 
  unsigned long dt = timeNow - timeLast;     // the time difference since last loop, microseconds
  // 
  float         dtS = timeNowS - timeLastS;  // a floating point version
  float err = dtS*1000000. - dt;             // error in dtS, microseconds
  static float maxErr = 0;                   // maximum magnitude error value seen so far
  maxErr = max(abs(err),maxErr);             // update the maximum error value for this time throughthe loop

  static unsigned long lastPrint = 0;        // Print out current status, but not every time
  // make the printing interval at least a tenth of a second, and irregularly longer as time goes on
  if((timeNow-lastPrint)/1000000. >= 0.1 + 0.0123456 * timeNowS){     // test if time to print in seconds
    lastPrint = timeNow;
    P(timeNowS,3);       // time to 3 places usually precise enough
    PCS(timeNow);        // microsecond versions are correct original data to within 4 us
    PCS(timeLast);
    PCS(dt);            
    PCS(timeNowS,7);     // floating point versions in seconds
    PCS(timeLastS,7);
    PCS(dtS,7);            
    // This error will be small for low values of timeNow but will get much larger over time.
    // You can predict this deterministically from the storage formats, or observe results:
    // By five minutes the error is around +/-15 microseconds (it varies).
    // By ten minutes the max error magnitude is around 110 us
    // By forty minutes the max error magnitude is around 370 us, and dtS is often zero!
    // By seventy minutes the max error is still about 370 us and micros() is about to wrap around to zero
    // By eighty minutes micros has wrapped around and the error is back down to around +/-15 us
    PCS(err,7);   
    PCSL(maxErr); // error in dtS microseconds
  }

  timeLast = timeNow;      // save the old value for next time through the loop
  timeLastS = timeNowS;
}
