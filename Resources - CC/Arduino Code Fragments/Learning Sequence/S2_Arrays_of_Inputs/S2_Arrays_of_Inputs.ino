#include "S.h"    // We can hide some of the clutter in another tab
#define NCH   3   // The number of analog channels to read, starting with A0, must be <= 6
unsigned a[NCH];  // Declare arrays etc. as globals so we can conveniently see them everywhere
float as[] = {0, 0, 0, 0, 0, 0}; // uninitialized variables make for hard to identify errors!
unsigned long timeNow = 0;
float timeNowS = 0.;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  P("\n\nRick's S2 -- The array version\n\n"); // worth printing out the name of the sketch to document
  P("Time [s], A0, smoothed A0, ...\n"); // provide headings to name the outputs
  pinMode(10, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  // Keep time in microseconds as an unsigned long for best accuracy.
  timeNow = micros();                    // the time we started this loop, microseconds
  timeNowS = timeNow / 1000000.;         // time now in seconds, float so it will have decimals
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds

  // get input and calculate values
  for (int i = 0; i < NCH; i++) a[i] = analogRead(A0 + i);
  byte led = digitalRead(13);         // 8 bit byte can hold whole numbers 0,1,2,...,255

  unsigned long tau = 100000;         // the smoothing time constant in microseconds bigger = smoother, slower
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g. 3333/10000 = 0
  float w = min(1., (float) dt / tau); // weight to give the latest reading of a in smoothing
  for (int i = 0; i < NCH; i++) {     // exponentially smoothed version of a, global so it persists
    as[i] = w * a[i] + (1 - w) * as[i];        // weighted average will smooth the data to reduce noise
  }                                   // use Ctrl-t or Cmd-t regularly to keep indentation correct

  // Note that i was declared inside the scope of the for() loop and doesn't exist here!
  // trying to print it will generate a compile error saying 'i' was not declared in this scope
//  PCSL(i);
  
  printLine();             // print out all the data 

  timeLast = timeNow;      // save the old value for next time through the loop
}

void printLine() {
  static unsigned long lastPrint = 0;// Print out current status, but not every time
  // this printing will take over 3 ms, but the rest of the loop only takes about 240 us
  if ((timeNow - lastPrint) / 1000000. >= 0.1) { // test if time to print in seconds
    lastPrint = timeNow;
    P(timeNowS, 3);      // time to 3 places usually precise enough
    for (int i = 0; i < NCH; i++) {
      PCS(a[i]);
      PCS(as[i], 3);     // Choose a whole block of lines and hit Ctrl-/ or Cmd-/ to comment/uncomment
    }
    P("\n");            // a separate newline will usually be tidier
  }
}
