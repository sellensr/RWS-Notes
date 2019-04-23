#ifndef Virtual217_h
#define Virtual217_h

#include <Arduino.h>

#define PSEUDO_GROUND   1.239
#define GAIN          100.0
#define USB_VOLTAGE     5.043
#define V33             3.296
#define V11             1.086

#ifndef INTERNAL      // dummy code to let it compile on M0 that has no INTERNAL mode
#define INTERNAL 123
#endif

extern float VrefX;

void analogReferenceV(unsigned src);
unsigned long analogReadV(unsigned port);
float randnV();

#endif    //Virtual217_h
