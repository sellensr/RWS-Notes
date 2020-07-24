// 2020-07-24 Use this code to simulate some calibration data for the potentiometer
#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()

#define LED_PIN 13
#define BUTTON_PIN 12
#define HI_PIN 2
#define vRef 3.300  // this voltage value should be adjusted to match your own hardware
#define rRef 10000  // this resistance value should be adjusted to match your own hardware

double AtomV = vRef / 65.535;     // convert 16 bit unsigned from ADC to mV 
double AtoV  = vRef / 65535.;     // convert 16 bit unsigned from ADC to V 

double potSet = 0;
unsigned cnt = 1;

void setup() {
  Serial.begin(115200);         // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.printf("\n\nHello World, L08 Starter\n\n");
  Serial.printf("Set your pot somewhere in the middle of it's range and run this code.\n\n");
  analogReadResolution(16);
  
  // Use A0 and A2 to supply the potentiometer so that the wiper on A1 is an adjustable voltage source.
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A2,HIGH);
  
  // collect some intial data for a zero reference
  for(int i=0;i<cnt;i++){
    potSet += analogRead(A1);
  }
  potSet /= cnt;
  
  Serial.printf("Average Potentiometer Reading is %8.1f\n",potSet);
  Serial.printf("Do not move the pot while generating simulated calibration data.\n\n"); 
}

unsigned long timeLast = 0;  // the last time we went through the loop, microseconds

void loop() {
  // some random values so everybody's cal data is different
  static double divPerDeg = 65535 / 380. + random(-3500,3500) / 1000.;
  static double divZero = 20000 + random(-15000,15000) / 100.; 
  static unsigned long ts = micros();
  unsigned long timeNow = micros();

  // Set the target angle to steps of 10 between 0 and 180 changing every second
  int targetAngle = (timeNow -ts) / 1000000;
  //targetAngle = targetAngle % 19; // uncomment to do the measurement repeatedly
  targetAngle *= 10;

  // If target has moved, reset the actual angle with some random variations for human error.
  // You could change this if you think you are more accurate at setting the positions.
  static double actualAngle = 0;
  if(abs(actualAngle-targetAngle) > 3) actualAngle = targetAngle + random(-1500,1500) / 1000.;

  // Get some actual analog data from the pot so it has noise typical of the system.
  double sum = 0;
  for(int i = 0;i<cnt;i++) sum += analogRead(A1);
  sum /= cnt;

  // Adjust the data to reflect the current actual angle being simulated
  unsigned ps = potSet;
  unsigned as = actualAngle * divPerDeg + divZero;
  double aRead = (sum - ps) + as; 
  double readAngle = (aRead - divZero) / divPerDeg;
  if(targetAngle <= 180) Serial.printf("%4.2f, %5d, %8.2f\n",timeNow/1000000.,targetAngle, aRead);
  timeLast = timeNow;        // save the old value for next time through the loop
  delay(100);
}
