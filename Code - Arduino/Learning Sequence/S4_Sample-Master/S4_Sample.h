#ifndef S4_SAMPLE_H   // make sure the .h file is only run once
#define S4_SAMPLE_H

#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <TimeLib.h>

// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println


// A class with a few functions as a sample library
class S4_Sample{
public:                           // accessible to everyone
  S4_Sample();                    // instantiate the class
  bool begin(unsigned long spd);  // initialize and set Serial speed
  void helloWorld();              // say hello
  unsigned long bytesFree();      // return an approximate number of bytes between the stack and the heap

protected:                        // accessible inside the class and derived classes
  bool initialized = false;       // has begin() completed successfully?

private:                          // accessible only inside this class
  unsigned long baud = 115200;
};

#endif //end the block on S4_SAMPLE_H
