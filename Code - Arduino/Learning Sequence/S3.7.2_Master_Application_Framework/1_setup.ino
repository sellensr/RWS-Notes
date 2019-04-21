void setup() {
  Serial.begin(115200);       // set the serial port speed
  Wire.begin();               // start the I2C library
  pinMode(BLINK_PIN,OUTPUT); 
  // INPUT_PULLUP mode uses the internal pullup resistor so you don't need to add one to the circuit
  pinMode(BUTTON_PIN,INPUT_PULLUP);     
  // Call buttonFall() automatically each time the button is pushed, falling from Vcc to ground
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonFall, FALLING);

  #if defined(ARDUINO_ARCH_SAMD)      // set the analog characteristics to match the processor            
    analogReadResolution(ANALOG_BITS);// analogReadResolution is not supported on UNO
    analogReference(AR_DEFAULT);      // Set it explicitly as a reminder, AR_DEFAULT for SAMD boards
  #elif defined(_AVR_)
    analogReference(DEFAULT);         // Set it explicitly as a reminder, DEFAULT for UNO
  #endif

  for(int i=0;i<1000;i++) analogRead(A0);   // Exercise the converter to clear any changes
  while(!Serial){}                          // Wait until the Serial port is ready to operate

  P("\n\nRick's Application Framework S3.7, Version: ");
  P(VERSION);   // worth printing out the name of the sketch and version to document
  #if defined(ARDUINO_ARCH_SAMD)
    P("\nARDUINO_ARCH_SAMD is defined, so using settings for SAMD processors.");
  #elif defined(_AVR_)
    P("\n_AVR_ is defined, so using settings for Arduino UNO R3 and similar processors.");
  #else
    P("\n not sure what the processor is, so behaviour may be strange...");
  #endif
  // provide headings to name the outputs
  P("\n\nTime [s], Loops, Digital Pins, Loop Rate [Hz], A0, A0 Smoothed, V0, ...\n"); 

  // Explicitly initialize all tasks and related variables for good form
  for(int i = 0; i<MAX_TASKS; i++){     // start out with all tasks empty, running very seldom
    fTask[i] = emptyTask;               // emptyTask() is a function that does nothing
    dtTaskMin[i] = 10.0;                // 10.0 seconds is a long time to go between updates
    lastTask[i] = 0;                    // time fTask[i] last called is zero microseconds
    lastTaskReturn[i] = 0;              // fTask[i] last returned nothing unusal
    lastTaskRT[i] = worstTaskRT[i] = 0; // fTask[i] took zero microseconds to run last time
  }

  // Put the high priority tasks at the top of the list to make sure they get run
  // on every pass through the loop. Later tasks in the list may get skipped if
  // the overall loop is taking too long to finish.
  fTask[0] = readTask;                  // Read the analog inputs from the first NCH ports       
  dtTaskMin[0] = 0.0;                   // Do first in loop() and every time through loop()       
  for(int i = 0;i<NCH;i++){             // Set starting smoothed values and time constants [seconds]
    as[i] = 0.0;
    tau[i] = 0.01;
  }
  
  fTask[1] = digTask;                  // Read the digital inputs from the first NCHD pins              
  dtTaskMin[1] = 0;                    // Do next in loop() and every time through loop()

  // now line up any other tasks in desired order of execution and priority

  fTask[2] = showTimesTask;    // Do somewhere in the middle at default repetition rate
  fTask[3] = BME280Task;
  fTask[4] = blinkPinTask;
  dtTaskMin[4] = 1.0;            
//  fTask[6] = unstagedTask;
//  fTask[7] = stagedTask; 
  
  fTask[SD_TASK] = sdTask;                // Write information to the SD card if present
  dtTaskMin[SD_TASK] = SD_TIME;           // Do second last in loop(), and not every time 

  fTask[CONSOLE_TASK] = consoleTask;      // Display information to the console
  dtTaskMin[CONSOLE_TASK] = CONSOLE_TIME; // Do last in loop(), and not every time
}
