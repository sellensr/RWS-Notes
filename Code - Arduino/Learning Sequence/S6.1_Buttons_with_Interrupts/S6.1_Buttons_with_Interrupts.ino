#include "af.h"

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;          // the time we started the current loop(), microseconds
float timeNowS = 0.0;               // timeNow, but in seconds, float so it will have decimals
unsigned long timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds
unsigned buttonCount = 0;

int bp(){  // ISR for a pushed button, may over count if button bounces
  buttonCount++;
}

void setup() {
  Serial.begin(115200);         // set the serial port speed
  // attachInterrupt(0,bp,FALLING);  // using the interrupt number directly can cause portability problems
  // Use digitalPinToInterrupt to do the conversion from pin number to interrupt number
  attachInterrupt(digitalPinToInterrupt(2),bp,FALLING);
  P("\n\nCounting Buttons\n\n");
  P("Time [s], Pushes since last time\n"); // provide headings to name the outputs
}

void loop() {
  timeNow = micros();                    // the time we started this loop, microseconds
  timeNowS = timeNow / 1000000.;         // time now in seconds, float so it will have decimals
  unsigned bc = buttonCount;             // retrieve and then reset the count right away
  buttonCount = 0;
  P(timeNowS);
  PCSL(bc);
  delay(1000);             // do nothing while the ISR tracks the pushes  
  timeLast = timeNow;      // save the old value for next time through the loop
}
