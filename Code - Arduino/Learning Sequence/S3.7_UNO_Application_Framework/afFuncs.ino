// These tasks probably won't change from one application to another.

// This task can be used to fill out the task list
int emptyTask(int iTask){    // do nothing in this task
  // iTask is the Task index so the task knows 
  return 0; // status code from task, 0 for nothing unusual
}

// This task will be useful in any application that reads from analog inputs
int readTask(int iTask){  // read and smooth current analog values -- call this task first
  static float vConversion = AREF / (ANALOG_MAX + 1.0); // Calculate only once for speed
  float dt = timeNow-lastTask[iTask];                   // time since last reading in microseconds
  for(int i = 0;i<NCH;i++){
    a[i] = analogRead(A0+i);                      // assumes analog channel numbers are contiguous
    float w = min(1.,dt / (tau[i] * 1000000.0));  // weight to give the latest reading of a in smoothing
    as[i] = w * a[i] + (1-w) * as[i];             // weighted average will smooth the data to reduce noise
    v[i] = (a[i] + 0.5) * vConversion;            // convert to voltages
    vs[i] = (as[i] + 0.5) * vConversion;
  }
  calcReadResults();
  return 0; // status code from task, 0 for nothing unusual
}

int showTimesTask(int iTask){
  P("\nLatest executions times for each task in the list [microseconds]\n");
  P(timeNowS,3);             // time the loop started
  for(int i = 0;i<MAX_TASKS;i++){
    PCS(lastTaskRunTime[i]);
  }
  P("\n\n");
  return 0; // status code from task, 0 for nothing unusual
}

#include <BME280I2C.h>

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

int BME280Task(int iTask){    // Read from the BME280
  // iTask is the Task index so the task knows 
  static bool initialized = false;
  if(!initialized){
    while(!bme.begin()){
      Serial.println("Could not find BME280 sensor!");
      delay(1000);
    }
    switch(bme.chipModel()){
       case BME280::ChipModel_BME280:
         if(DEBUG) Serial.println("Found BME280 sensor! Success.");
         break;
       case BME280::ChipModel_BMP280:
         if(DEBUG) Serial.println("Found BMP280 sensor! No Humidity available.");
         break;
       default:
         if(DEBUG) Serial.println("Found UNKNOWN sensor! Error!");
    } 
    initialized = true; 
  }

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);

  bme.read(pres, temp, hum, tempUnit, presUnit);

  return 0; // status code from task, 0 for nothing unusual
}
