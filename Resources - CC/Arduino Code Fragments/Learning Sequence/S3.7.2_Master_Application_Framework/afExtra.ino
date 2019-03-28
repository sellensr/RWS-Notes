void buttonFall(){  // ISR to be called by interrupt whenever the button is pushed
  static unsigned long lastSwitch = 0;
  if(micros()-lastSwitch > 300000){    // don't switch on a bounce, or on release
    lastSwitch = micros();
    // Make the console go fast as possible, or switch back to normal speed
    if(dtTaskMin[CONSOLE_TASK] != 0) dtTaskMin[CONSOLE_TASK] = 0; 
    else dtTaskMin[CONSOLE_TASK] = CONSOLE_TIME; 
    // Make the sd card log file go fast as possible, or switch back to normal speed
    if(dtTaskMin[SD_TASK] != 0) dtTaskMin[SD_TASK] = 0; 
    else dtTaskMin[SD_TASK] = SD_TIME; 
  }
}

// This seems to work OK on UNO, but beyond that the results are dubious.
unsigned long bytesFree(){    // approximate number of bytes between the heap and the stack
  static byte sz = 1;         // malloc a small chunk -- byte size will rollover after 255
  char dummy;                 // a variable on the stack at a higher address
  char *c,*d;                 // two byte sized pointers
  d = &dummy;                 // point to the stack variable
  c = (char *) malloc(sz++);  // point to the malloc'd space which should be lower
  if(sz > 24) sz = 1;         // keep sz really small, but moving around
  unsigned long b = d - c;    // unsigned long version
  free(c);                    // make sure to free the malloc() to avoid leaks
  return b;
}
