#include "S4_Sample.h"

S4_Sample::S4_Sample() {}

bool S4_Sample::begin(unsigned long spd){
  initialized = false;                  // not done yet
  baud = spd;                           // set the serial port speed
  Serial.begin(baud);                   // start the serial port
  while(!Serial && millis() < 5000);    // wait up to 5 seconds for connection
  initialized = true;                   // initialization has been completed at least once
  return initialized;                   // report back to the calling function
}

void S4_Sample::helloWorld(){
  P("Hello World! ");
  P(millis());
  P(" since processor started\n");
}

unsigned long S4_Sample::bytesFree() {  // approximate number of bytes between the heap and the stack
  static byte sz = 1;         // malloc a small chunk
  char dummy;                 // a variable on the stack at a higher address
  char *c, *d;                // two byte sized pointers
  d = &dummy;                 // point to the stack variable
  c = (char *) malloc(sz++);  // point to the malloc'd space which should be lower
  unsigned long b = d - c;    // unsigned long version
  free(c);                    // make sure to free the malloc() to avoid leaks
  return b;
}
