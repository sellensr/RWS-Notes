#include <BME280I2C.h>  // This BME280 library works with the generic BME280 breakouts
                        // sourced on eBay -- the adafruit library doesn't.
                        // The code is adapted directly from their library example.

BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

int BME280Task(int iTask){    // Read from the BME280
  // iTask is the Task index so the task knows 
  static bool initialized = false;
  static bool found = false;
  if(!initialized){           // This is the first time the function has been called
    if(!bme.begin()) Serial.println("Could not find BME280 sensor!");
    else{
      switch(bme.chipModel()){
         case BME280::ChipModel_BME280:
           if(DEBUG) Serial.println("Found BME280 sensor! Success.");
           found = true;
           break;
         case BME280::ChipModel_BMP280:
           if(DEBUG) Serial.println("Found BMP280 sensor! No Humidity available.");
           found = true;
           break;
         default:
           if(DEBUG) Serial.println("Found UNKNOWN sensor! Error!");
      }
    } 
    initialized = true; 
  }
  if(found){
    BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
    BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  
    bme.read(pres, temp, hum, tempUnit, presUnit);
  
    return 0;       // status code from task, 0 for nothing unusual
  }
  else return -1;   // there is no BME280 data available
}
