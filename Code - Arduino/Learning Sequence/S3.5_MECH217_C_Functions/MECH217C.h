#ifndef _MECH217C_h     // prevent the header from being included multiple times
#define _MECH217C_h

// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;   // the time we started the current loop(), microseconds
float timeNowS = 0;          // timeNow, but in seconds
unsigned long timeLast = 0;  // the last time we went through the loop(), microseconds

#endif // _MECH217C_h
