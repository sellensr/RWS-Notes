#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds
int meterMode = 0;  // 0 for DC voltage, 1 for resistance/continuity


#define BUTTON_PIN 12
#define HI_PIN 2
#define vRef 3.300

double AtomV = vRef / 65.535;     // convert 16 bit unsigned from ADC to mV 
double AtoV  = vRef / 65535.;     // convert 16 bit unsigned from ADC to V 

void setup() {
  Serial.begin(300);      // M0 USB is fast no matter what speed we pick
  while(!Serial && millis() < 5000);
  Serial.print("\n\nS2.3 Multi-Meter Output on Serial Monitor\n");
  Serial.print(    "=========================================\n\n");
  Serial.print("Put a reference resistor (minimum 2K) between pin 2 and pin A5.\n");
  Serial.print("Plug a potentiometer into pins A0, A1, A2 to provide an adjustable voltage.\n");
  Serial.printf("Push the button to pull pin %d low and switch metering modes.\n",
                BUTTON_PIN);
  Serial.print("Use Pin A4 as the positive lead and Pin A3 as the negative lead for DC Voltage Mode.\n");
  Serial.print("Put your resistance between Pin A5 and Ground for Resistance Mode.\n\n");
  delay(2000);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(HI_PIN,OUTPUT);
  digitalWrite(HI_PIN,HIGH);
  analogReadResolution(16);
//  analogReference(AR_INTERNAL1V65);
  // Use A0 and A2 to supply the potentiometer so that the wiper on A1 is an adjustable voltage source.
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A2,HIGH);
  setupMode(meterMode);
}

void setupMode(int mode) {     
  // Use A0 and A2 to supply the potentiometer so that the wiper on A1 is an adjustable voltage source.
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A2,HIGH);
}

void loop() {
  static unsigned long lastPrint = 0, lastLegend = 0, lastSerial1 = 0;
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();      // the time we started this loop, microseconds
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds
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
    Serial.printf("  Resistance 5/GND = %8.0f Ohms (for 10K ref)\n",
                  a[5] / (vRef - a[5]) * 10000);   
    Serial.printf("\n");   
  }
  timeLast = timeNow;      // save the old value for next time through the loop
  delay(3);
}