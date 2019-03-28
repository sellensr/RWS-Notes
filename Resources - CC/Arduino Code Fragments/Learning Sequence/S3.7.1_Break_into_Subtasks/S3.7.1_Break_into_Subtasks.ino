#include "af.h"
#define NCH 3   

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;          // the time we started the current loop(), microseconds
float timeNowS = 0.0;               // timeNow, but in seconds, float so it will have decimals
unsigned long timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds

unsigned a[NCH];

void readTask(){  // read current analog values -- call this task first
  for(int i = 0;i<NCH;i++) a[i] = analogRead(A0+i); 
}

void consoleTask(){  // report to the serial monitor -- call this task last
  P(timeNowS,3);
  for(int i = 0;i<NCH;i++){ 
    PCS(a[i]);
  }
  P("\n");
}

void setup() {
  Serial.begin(115200);     // set the serial port speed
  P("\n\nRick's Applicatio Framework S3.7.1 Break into Subtasks\n\n");
  P("Time [s], A0, A1, ...\n"); // provide headings to name the outputs
}

// The loop() function is much smaller when the tasks are broken out into 
// separate functions, like readTask() and consoleTask() here.
void loop() {
  timeNow = micros();                    // the time we started this loop, microseconds
  timeNowS = timeNow / 1000000.;         // time now in seconds, float so it will have decimals
  
  readTask();
  consoleTask();
  
  timeLast = timeNow;      // save the old value for next time through the loop
}
