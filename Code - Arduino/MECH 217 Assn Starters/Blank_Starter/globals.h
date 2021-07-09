// This sketch will contain the core code for your work in the course.
// Make sure you keep an up to date copy, and correct any problems revealed 
// in your assignments. This code will be the basis for some test answers.

#include <RTClib.h>           // Because Itsy Bitsy M0 needs it for printf("%8.3f...
#include "RWS_UNO.h"          // Mostly for the i2c support
RWS_UNO uno = RWS_UNO();      // an Arduino utilities object

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds
unsigned long timeNow = micros();      // the time we started this loop, microseconds
unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds

#define N_ANALOG     6        // total number of analog pins to allow for
#define BUTTON_PIN  12        // the pin with the button
#define LED_PIN     13        // the pin with the built-in LED
#define LOW_PIN 10
#define HI_PIN 2
#define vRef 3.300  // this voltage value should be adjusted to match your own hardware
#define rRef 10000  // this resistance value should be adjusted to match your own hardware

double AtomV = vRef / 65.535;     // convert 16 bit unsigned from ADC to mV 
double AtoV  = vRef / 65535.;     // convert 16 bit unsigned from ADC to V 
