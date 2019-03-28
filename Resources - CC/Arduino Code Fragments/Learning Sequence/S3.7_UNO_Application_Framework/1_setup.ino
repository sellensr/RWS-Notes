void setup() {
  Serial.begin(115200);     // set the serial port speed
  Wire.begin();             // start the I2C library

#ifdef ANALOG_GT_10         // analogReadResolution is not supported on UNO
  analogReadResolution(ANALOG_BITS);
#endif

  analogReference(AR_DEFAULT);              // Set it explicitly as a reminder, DEFAULT or AR_DEFAULT
  for(int i=0;i<1000;i++) analogRead(A0);   // Exercise the converter to clear any changes
  while(!Serial){};                         // Wait until the Serial port is ready to operate

  P("\n\nRick's Application Framework S3.7\n\n"); // worth printing out the name of the sketch to document
  P("Time [s], Loop Rate [Hz], A0, A0 Smoothed, V0, ...\n"); // provide headings to name the outputs
  
  for(int i = 0; i<MAX_TASKS; i++){     // start out with all tasks empty, running very seldom
    fTask[i] = emptyTask;               // emptyTask() is a function that does nothing
    dtTaskMin[i] = 10.0;                // 10.0 seconds is a long time to go between updates
    lastTask[i] = 0;                    // time fTask[i] last called is zero microseconds
    lastTaskReturn[i] = 0;              // fTask[i] last returned nothing unusal
    lastTaskRunTime[i] = 0;             // fTask[i] took zero microseconds to run last time
  }
  
  fTask[0] = readTask;                  // Do first in loop() and every time through loop()             
  dtTaskMin[0] = 0;           
  for(int i = 0;i<NCH;i++){             // Set starting smoothed values and time constants [seconds]
    as[i] = 0.0;
    tau[i] = 0.01;
  }                                     // now line up any other tasks in desired order of execution
  
  fTask[CONSOLE_TASK] = consoleTask;    // do last in loop(), and not every time
  dtTaskMin[CONSOLE_TASK] = CONSOLE_TIME; 
  fTask[SD_TASK] = sdTask; 
  dtTaskMin[SD_TASK] = SD_TIME;           

  if(MAX_TASKS > 9){
    fTask[1] = BME280Task;
//    fTask[2] = unstagedTask;
    pinMode(BLINK_PIN,OUTPUT); 
    // Interesting that this sometimes works even though blinkPin() doesn't take 
    // an argument or return a value. Generates an error comiling for the M0           
    //fTask[5] = blinkPin;                  // do somewhere in the middle at default repetition rate             
    fTask[6] = showTimesTask;
//    fTask[7] = stagedTask; 
  }

  // INPUT_PULLUP mode uses the interal pullup resistor so you don't need to add one to the circuit
  pinMode(BUTTON_PIN,INPUT_PULLUP);     
  // Call buttonFall() automatically each time the button is pushed, falling from +5 to ground
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonFall, FALLING);
}
