#include "s.h"

unsigned gridMillis = 100;        // number of milliseconds between grid boxes
unsigned maxGrid = 1023;          // how high is the grid
unsigned microsPerStep = 1000;    // M0 seems to deliver reliably at 1 ms / step

void setup() {
  Serial.begin(300);      // M0 USB is fast no matter what speed
  // Even at 300 baud, serial signals need an oscilloscope, but analog may be viewable.
  Serial1.begin(300);    // Serial1 is at set speed -- try watching it on pin 9 or 10
  while(!Serial && millis() < 5000);
  P("\n\nS2.2 Scope-Like Output on Serial Plotter\n========================================\n\n");
  P("Showing offset traces for pins 9 and 10, and unsigned plots for A0 to A3.\n");
  P("Push the button to pull pin "); P(BUTTON_PIN); P(" low and pause the output for viewing.\n");
  P("Close the monitor and open Tools/Serial Plotter to see the graphical output.\n\n");
  P("Grid_Spacing ~"); P(gridMillis); P("ms D9 D10 A0 A1 A2 A3\n");
  delay(2000);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
}

void loop() {
  static unsigned long lastPrint = 0, lastLegend = 0, lastSerial1 = 0;
  int grid1 = 0, grid2 = 0;
  if(millis()%(2*gridMillis) >= gridMillis){    // swap every gridMillis
    grid1 = maxGrid * 1.05;       // a little above the top of the grid at maxGrid
    grid2 = maxGrid * -0.05;      // a little below the bottom of the grid at 0
  } else {
    grid1 = maxGrid * -0.05;
    grid2 = maxGrid * 1.05;
  }
  double t = micros() / 1000000.;
  if (micros() - lastLegend > microsPerStep * 2000) { // print legend now and then
    lastLegend = micros();
    P("Grid_Spacing ~"); P(gridMillis); P("ms D9 D10 A0 A1 A2 A3\n");
  }
  if (micros() - lastSerial1 > 1500000) {   // send a little to Serial1 now and then
    lastSerial1 = micros();
    Serial1.print("A few characters will be hard to decode..."); 
  }
  if (millis() % 2000 > 1000 && micros() - lastPrint > microsPerStep && digitalRead(BUTTON_PIN)) {
    lastPrint = micros();
    P(grid1);     // plot some grid lines
    PCS(grid2);
    PCS((digitalRead(9) * maxGrid + maxGrid)/2.2);
    PCS((digitalRead(10) * maxGrid + maxGrid)/2.4);
    PCS(analogRead(A0));
    PCS(analogRead(A1));
    PCS(analogRead(A2));
    PCS(analogRead(A3));
//    PCS(millis()%1000);   // useful to see how often printing actually happens
    PL();
  }
}
