#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds
unsigned long timeNow = micros();      // the time we started this loop, microseconds
unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds
int meterMode = 0;  // 0 for DC voltage, 1 for resistance/continuity


#define LED_PIN 13
#define BUTTON_PIN 12
#define LOW_PIN 10
#define HI_PIN 2
#define vRef 3.300  // this voltage value should be adjusted to match your own hardware
#define rRef 10000  // this resistance value should be adjusted to match your own hardware

double AtomV = vRef / 65.535;     // convert 16 bit unsigned from ADC to mV 
double AtoV  = vRef / 65535.;     // convert 16 bit unsigned from ADC to V 

void setup() {
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,HIGH);
  Serial.begin(300);      // M0 USB is fast no matter what speed we pick
  while(!Serial && millis() < 5000);
  Serial.print("\n\nS2.3 Multi-Meter Output on Serial Monitor\n");
  Serial.print(    "=========================================\n\n");
  Serial.print("Put a reference resistor (minimum 2K) between pin 2 or +3.3 and pin A5.\n");
  Serial.print("Plug a potentiometer into pins A0, A1, A2 to provide an adjustable voltage.\n");
  Serial.printf("Push the button to pull pin %d low and update metering quickly.\n",
                BUTTON_PIN);
  Serial.print("Use Pin A4 as the positive lead and Pin A3 as the negative lead for Differential DC Voltage.\n");
  Serial.print("Put your resistance between Pin A5 and Ground for Resistance Measurements.\n\n");
  delay(2000);
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,HIGH);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(HI_PIN,OUTPUT);
  digitalWrite(HI_PIN,HIGH);
  pinMode(LOW_PIN,OUTPUT);
  digitalWrite(LOW_PIN,LOW);
  analogReadResolution(16);
//  analogReference(AR_INTERNAL1V65);
  setupMode(meterMode);
  digitalWrite(LED_PIN,LOW);
}

void setupMode(int mode) {     
  // Use A0 and A2 to supply the potentiometer so that the wiper on A1 is an adjustable voltage source.
  // Make A2 a ground pin so you can just plug in the pot on A0/A1/A2 and it will follow A0 output
  pinMode(A2,OUTPUT);
  digitalWrite(A2,0);
  // Start with A0 at +3.3V for the pot
  analogWrite(A0,1023);
}

void loop() {
  timeLast = timeNow;      // save the old value before changing timeNow
  static unsigned long lastPrint = 0, lastLegend = 0, lastSerial1 = 0;
  // Keep time in microseconds as an unsigned long for best accuracy.
  timeNow = micros();      // the time we started this loop, microseconds
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  unsigned long tau = 100000;            // the smoothing time constant in microseconds bigger = smoother, slower
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g. 3333/10000 = 0
  float w = min(1.,(float) dt/tau);      // weight to give the latest reading of a in smoothing
  double a[8] = {0};
  double a2[8] = {0};

  // This is a great example of where a hard coded number like 1000 will break
  // your code later if you decide to average over a different number of samples,
  // but don't remember to change the number in all the locations -- use #define!
  for(int j = 0;j < 1000;j++){
    double v1 = analogRead(A1) * AtoV;
    double v3 = analogRead(A3) * AtoV;
    double v4 = analogRead(A4) * AtoV;
    double v5 = analogRead(A5) * AtoV;
    double v6 = v4 - v3;
    a[1] += v1;
    a2[1] += v1 * v1;
    a[3] += v3;
    a2[3] += v3 * v3;
    a[4] += v4;
    a2[4] += v4 * v4;
    a[5] += v5;
    a2[5] += v5 * v5;
    a[6] += v6;
    a2[6] += v6 * v6;
  }
  for(int i = 0; i < 7; i++){
    a[i] /= 1000.;
    a2[i] /= 1000.;
  }

  // continuity indicator
  if(a[5]/vRef < 0.02) digitalWrite(LED_PIN,HIGH);
  else digitalWrite(LED_PIN,LOW);
  
  double t = timeNow / 1000000.;
  if (micros() - lastLegend > 10000000L) { // print legend now and then
    lastLegend = micros();
  }
  if (micros() - lastPrint > 2000000 || !digitalRead(BUTTON_PIN)) {
    lastPrint = micros();
    Serial.printf("t = %8.3f s", t);
    Serial.printf(", V1 = %8.4f V DC (%7.2f mV)\n",
                  a[1], sqrt(a2[1] - a[1]*a[1]) * 1000.);   
    Serial.printf("                V3 = %8.5f V DC (%7.2f mV)\n",
                  a[3], sqrt(a2[3] - a[3]*a[3]) * 1000.);   
    Serial.printf("                V4 = %8.5f V DC (%7.2f mV)\n",
                  a[4], sqrt(a2[4] - a[4]*a[4]) * 1000.);   
    Serial.printf("                V5 = %8.5f V DC (%7.2f mV)\n",
                  a[5], sqrt(a2[5] - a[5]*a[5]) * 1000.);   
    Serial.printf("          Diff 3/4 = %8.5f V DC (%7.2f mV)\n",
                  a[6], sqrt(a2[6] - a[6]*a[6]) * 1000.);   
    Serial.printf("  Resistance 5/GND = %8.0f Ohms (rRef = %6.0f Ohm)\n",
                  a[5] / (vRef - a[5]) * rRef, (double) rRef);   
    Serial.printf("\n");   
  }
  delay(3);
}
