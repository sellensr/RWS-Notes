/* See http://arduino.land/FAQ/content/7/42/en/how-to-use-multiple-tabs.html
 *  
 * You can use the tab feature to organize your code. The IDE will 
 * create files for each tab.
 * 
 * Naming Tabs
 * 
 * Use a name like t2 for a tab (no extension like .h or .c) and it 
 * will be saved as e.g. t2.ino in your sketch directory. The IDE 
 * will use all of the .ino files in your sketch together to
 * build your sketch, as if they were all on a single page.
 * 
 * Use a name with the extension .h for a header file, usually defining
 * common constants or declaring functions.
 * 
 * Use a name with an extension .c for some C code, perhaps defining 
 * functions that will be called from your setup() or loop() functions.
 * 
 * Adding Files
 * 
 * Choose Sketch/Add File to import the tabs you created in a previous 
 * sketch. This adds a copy of the other file that you can change without
 * altering the original. See S3.5 for a better way to include the same 
 * code in multiple projects and keep it up to date.
 */
 
#include "S.h"    // Include files with .h and .c extensions manually  
#include "test.c" // Simple, but not fully portable...

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  while(!Serial && millis() < 5000);
  P("\n\nRick's S1.3\n\n"); // worth printing out the name of the sketch to document
  P("Time [s], Time [us], Time [us], Pin 10, A0, dt, w, A0 Smoothed\n"); // provide headings to name the outputs
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();      // the time we started this loop, microseconds
  unsigned long timeNow2 = t2();         // just a test
  unsigned long timeNow3 = t3();         // move along
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
  
  if(led == 1) digitalWrite(13,0);
  else digitalWrite(13,1);

  P(timeNowS,3);           // Print out current status, time to 3 places usually precise enough
  PCS(timeNow2);
  PCS(timeNow3);
  PCS(d);
  PCS(a);
  PCS(dt);
  PCS(w,5);
  PCSL(as,5);

  timeLast = timeNow;      // save the old value for next time through the loop

  // The only time to use delay is to slow down a sketch so you can see what is going on
  delay(0);   // delay() is bad -- drone crashes happen in split seconds!
}
