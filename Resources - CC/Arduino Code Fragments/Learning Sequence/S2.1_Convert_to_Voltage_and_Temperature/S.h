// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println

// Defining some more macros will make it a lot easier to print 
// out your results as you go along

// Debug versions will only print if DEBUG is non-zero
//#define DPCS if(DEBUG)Serial.print(", ");if(DEBUG)Serial.print
#define DP if(DEBUG) P
#define DPCS if(DEBUG) PCS
#define DPCSL if(DEBUG) PCSL

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds

