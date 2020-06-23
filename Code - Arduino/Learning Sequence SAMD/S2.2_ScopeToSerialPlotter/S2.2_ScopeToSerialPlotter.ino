#include "s.h"

unsigned gridMillis = 100;        // number of milliseconds between grid boxes
unsigned maxGrid = 3300;          // how high is the grid
unsigned microsPerStep = 2000;    // M0 seems to deliver fairly reliably at 2 ms / step
double AtomV = 3300 / 65535.;     // convert 16 bit unsigned to mV 

bool couplingAC = true;           // set true to see AC portion only
bool showDigital = false;         // set true to show pins 9 and 10
int channels = 4;                 // number of analog channels to display

void setup() {
  Serial.begin(300);      // M0 USB is fast no matter what speed
  // Even at 300 baud, serial signals need an oscilloscope, but analog may be viewable.
  Serial1.begin(300);    // Serial1 is at set speed -- try watching it on pin 9 or 10
  while(!Serial && millis() < 5000);
  Serial.printf("\n\nS2.2 Scope-Like Output on Serial Plotter\n");
  Serial.printf(    "========================================\n\n");
  Serial.printf("Showing offset traces for pins 9 and 10, and unsigned plots for A0 to A3.\n");
  Serial.printf("Push the button to pull pin %d low and pause the output for viewing.\n",
                BUTTON_PIN);
  Serial.printf("Close the monitor and open Tools/Serial Plotter to see the graphical output.\n\n");
  Serial.printf("Grid_Spacing ~%dms D9 D10 A0 A1 A2 A3\n", gridMillis);
  delay(2000);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  analogReadResolution(16);
}

void loop() {
  static unsigned long lastPrint = 0, lastLegend = 0, lastSerial1 = 0;
  int grid1 = 0, grid2 = 0;
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();      // the time we started this loop, microseconds
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds
  unsigned long tau = 100000;            // the smoothing time constant in microseconds bigger = smoother, slower
  if(couplingAC) tau *= 10;
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g. 3333/10000 = 0
  float w = min(1.,(float) dt/tau);      // weight to give the latest reading of a in smoothing
  double a[6] = {0};
  static double as[6] = {0};

  for(int i = 0; i < 4; i++){
    a[i] = analogRead(A0+i) * AtomV;
    if(as[i] != 0) as[i] = (1. - w) * as[i] + w * a[i];
    else as[i] = a[i];    
  }
  int gl = maxGrid * -0.01;
  int gh = maxGrid *  1.01;
  if(couplingAC) gh = abs(gl);
  if(millis()%(2*gridMillis) >= gridMillis){    // swap every gridMillis
    grid1 = gh;       // a little above the top of the grid at maxGrid
    grid2 = gl;      // a little below the bottom of the grid at 0
  } else {
    grid1 = gl;
    grid2 = gh;
  }
  double t = micros() / 1000000.;
  if (micros() - lastLegend > microsPerStep * 2000) { // print legend now and then
    lastLegend = micros();
  Serial.printf("Grid_Spacing~%dms Values_in_mV.... ", gridMillis);
  if(showDigital) Serial.printf("D9.... D10.... ");
//  Serial.printf("A0.... A0s.... A1.... A1s.... A2.... A2s.... A3.... A3s....\n");
  for(int i = 0; i < channels; i++) Serial.printf("A%d.... A%ds.... ", i, i);
  Serial.printf("\n");
  }
  if (micros() - lastSerial1 > 1500000) {   // send a little to Serial1 now and then
    lastSerial1 = micros();
    Serial1.printf("A few characters will be hard to decode..."); 
  }
  if (millis() % 2000 > 1000 && micros() - lastPrint > microsPerStep && digitalRead(BUTTON_PIN)) {
    lastPrint = micros();
    Serial.printf("%d, %d",
                  grid1, grid2); // vertical grid lines
    if(showDigital) Serial.printf(", %.2f, %.2f",
                  (digitalRead(9) * maxGrid + maxGrid)/2.2,   // digital values
                  (digitalRead(10) * maxGrid + maxGrid)/2.4); // digital values                
    for(int i = 0; i < channels; i++) {
      if(couplingAC) Serial.printf(", %.2f, %.2f",
                    a[i]- as[i], 0.0); // analog values                
      else Serial.printf(", %.2f, %.2f",
                    a[i], as[i]); // analog values   
    }         
    Serial.printf("\n");   
  }
  timeLast = timeNow;      // save the old value for next time through the loop
}
