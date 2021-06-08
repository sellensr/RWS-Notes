// Don't include the SD card segment for the UNO without being very careful
// around the question of having enough memory available. Including this SD
// code adds hundreds of bytes to the storage required and will push an UNO 
// over the edge with the tiny 2K RAM available.
#if defined(ARDUINO_ARCH_SAMD)      // exists for SAMD boards like the M0

#include <SD.h> 

// set chip select pin to 4 for Feather M0 Adalogger
// set to 10 for Adafruit Logger card or 4 for 1.8" TFT UNO Shields
#define SD_CS    4                

File logfile;  // the file handle for writing the data to on the SD card

// similar to P() and PCS() except going to the log file instead of Serial
#define LOGP logfile.print
#define LOGPCS logfile.print(", ");logfile.print

int sdTask(int iTask){    // Write to the SD Card
  // iTask is the Task index so the task knows 
  static bool initialized = false;
  static unsigned long lastFlush = 0;
  if(!initialized){       // Run the setup only once
    sdSetup();
    initialized = true; 
  }
  if (! logfile) return -1;     // there is no file, so don't waste time
  
  LOGP(timeNowS,3);             // time the loop started
  LOGPCS(1/loopTime,0);         // typical looping rate in Hz
  for(int i = 0;i<NCH;i++){     // the collected data from the analog channels
//    LOGPCS(a[i]);               // raw
//    LOGPCS(as[i]);              // smoothed
//    LOGPCS(v[i],4);             // as voltage
//    LOGPCS(vs[i],4);
//    LOGPCS(x[i],4);             // as a followon result, e.g. temperature
    LOGPCS(xs[i],4);
  }
  LOGPCS(temp);
  LOGPCS(hum);
  LOGPCS(pres);
  LOGP("\n");
  // Writing the buffer out to the SD card can take many miliiseconds and 
  // doesn't need to be done every time.
  if(timeNow-lastFlush > 60000000){ // make sure SD updated at least every minute
    logfile.flush();
    lastFlush = timeNow;
  }
  return 0;
}

void sdSetup(void){
    // initialize the SD card
    P("SD...");
    // see if the card is present and can be initialized:
    if (!SD.begin(SD_CS)) P("SD fail");
    P("Init...");
  
    char filename[] = "LOG00000.CSV";      // Look for the first available file name
    for (uint32_t i = 0; i < 100000; i++) {
      filename[3] = i/10000%10 + '0';
      filename[4] = i/1000%10 + '0';
      filename[5] = i/100%10 + '0';
      filename[6] = i/10%10 + '0';
      filename[7] = i%10 + '0';
      if (! SD.exists(filename)) {        // only open a new file if it doesn't exist
        logfile = SD.open(filename, FILE_WRITE); 
        break;                            // stop looking and leave the for() loop!
      }
    }
  
    if (! logfile) P("couldn't create file ");
    P(filename);
    P("\n"); 
}

#else

int sdTask(int iTask){    // We don't think you have the resources!
  return -1;
}

#endif
