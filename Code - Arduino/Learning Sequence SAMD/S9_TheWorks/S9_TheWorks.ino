#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_LIS3DH.h>

Adafruit_BME280 bme280;
Adafruit_BMP280 bmp280;
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_BMP085 bmp180;

bool foundBMP180 = false, foundBME280 = false, foundLIS = false, foundBMP280 = false;

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds
int meterMode = 0;  // 0 for DC voltage, 1 for resistance/continuity


#define BUTTON_PIN 12
#define HI_PIN 2
#define vRef 3.300
#define N_AVG 1

double AtomV = vRef / 65.535;     // convert 16 bit unsigned from ADC to mV 
double AtoV  = vRef / 65535.;     // convert 16 bit unsigned from ADC to V 

void setup() {
  Serial.begin(300);      // M0 USB is fast no matter what speed we pick
  while(!Serial && millis() < 5000);
  Serial.print("\n\nS9 Multi-Meter Output on Serial Monitor plus some I2C sensor testing\n");
  Serial.print(    "======================================================================\n\n");
  Serial.print("Put a reference resistor (minimum 2K) between pin 2 and pin A5.\n");
  Serial.print("Plug a potentiometer into pins A0, A1, A2 to provide an adjustable voltage.\n");
  Serial.printf("Push the button to pull pin %d low and produce faster output.\n",
                BUTTON_PIN);
  Serial.print("Use Pin A4 as the positive lead and Pin A3 as the negative lead for DC Voltage Mode.\n");
  Serial.print("Put your resistance between Pin A5 and Ground for Resistance Mode.\n");
  Serial.print("Add A BMP180 (or BME280, or BMP280 but only one) and/or an LIS3DH on I2C.\n\n");
  delay(2000);
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(HI_PIN,OUTPUT);
  digitalWrite(HI_PIN,HIGH);
  analogReadResolution(16);
//  analogReference(AR_INTERNAL1V65);
  setupI2C(); // comment out to skip the I2C sensors
}

void setupI2C(){
  // find a pressure sensor if there is one
  if (bmp180.begin()){ 
    foundBMP180 = true;
    Serial.print("Found a BMP180 (or BMP085) sensor!\n");  
  }
  else {
    Serial.println("Could not find a valid BMP180 sensor, trying BME280!");
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    unsigned statusBME280 = bme280.begin();  
    if(!statusBME280) statusBME280 = bme280.begin(0x76);
    if (!statusBME280) {
        Serial.println("Could not find a valid BME280 sensor on 0x76 or 0x77, trying BMP280!");
        Serial.print("SensorID was: 0x"); Serial.println(bme280.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        unsigned statusBMP280 = bmp280.begin();
        if(!statusBMP280) statusBMP280 = bmp280.begin(0x76);
        if(!statusBMP280) {
          Serial.println("Could not find a valid BMP280 sensor on 0x77 or 0x76.");
        } else {
          foundBMP280 = true;
          Serial.print("Found a BMP280 sensor!\n");  
        }
    } else {
      foundBME280 = true; 
      Serial.print("Found a BME280 sensor!\n");  
    }
  }

  // find an accelerometer if there is one
  unsigned statusLIS3DH = lis.begin(0x18);
  if(!statusLIS3DH) statusLIS3DH = lis.begin(0x019);  // alternative i2c address
  if (!statusLIS3DH) {
    Serial.println("Could not start LIS3DH");
  } else {
    foundLIS = true;
    Serial.println("LIS3DH found!");
    lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!
    Serial.print("Range = "); Serial.print(2 << lis.getRange());  
    Serial.println("G");
  }

}

void loop() {
  static unsigned long lastPrint = 0, lastLegend = 0, lastSerial1 = 0;
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();      // the time we started this loop, microseconds
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds
  unsigned long tau = 100000;            // the smoothing time constant in microseconds bigger = smoother, slower
  // since dt usually less than tau, we must cast the calculation as (float) to avoid e.g. 3333/10000 = 0
  float w = min(1.,(float) dt/tau);      // weight to give the latest reading of a in smoothing
  double a[8] = {0};
  double a2[8] = {0};
  // values will be zero unless there's a sensor to provide them
  double temperature = 0, pressure = 0, humidity = 0, gx = 0, gy = 0, gz = 0;

  // get analog data mean and rms
  for(int j = 0;j < N_AVG;j++){
    double v[8] = {0};
    for(int i = 0;i < 6;i++){
      v[i] = analogRead(A0 + i) * AtoV;
      a[i] += v[i];
      a2[i] += v[i] * v[i];
    }
    // differential voltage data for A3/A4
    v[6] = v[4] - v[3];
    a[6] += v[6];
    a2[6] += v[6] * v[6];
  }
  for(int i = 0; i < 7; i++){
    a[i] /= N_AVG;
    a2[i] /= N_AVG;
  }

  // get pressure / temperature / humidity data if available
  if(foundBME280){
    temperature = bme280.readTemperature();
    pressure = bme280.readPressure();    
    humidity = bme280.readHumidity();    
  } else if(foundBMP280){
    temperature = bmp280.readTemperature();
    pressure = bmp280.readPressure();   
  } else if(foundBMP180){
    temperature = bmp180.readTemperature();
    pressure = bmp180.readPressure();
  }

  // get accelerometer data if available
  if(foundLIS){
    sensors_event_t event; 
    lis.getEvent(&event);
    gx = event.acceleration.x;
    gy = event.acceleration.y;
    gz = event.acceleration.z;
  }
  
  double t = timeNow / 1000000.;
  if (micros() - lastLegend > 10000000L) { // print legend now and then
    lastLegend = micros();
  }
  if (micros() - lastPrint > 2000000 && !digitalRead(BUTTON_PIN)) {
    lastPrint = micros();
    Serial.printf("t = %8.3f s", t);
    Serial.printf(", V1 = %8.4f V DC (%7.2f mV)\n",
                  a[1], sqrt(a2[1] - a[1]*a[1]) * 1000.);   
    Serial.printf("                V3 = %8.5f V DC (%7.2f mV)\n",
                  a[3], sqrt(a2[3] - a[3]*a[3]) * 1000.);   
    Serial.printf("                V4 = %8.5f V DC (%7.2f mV)\n",
                  a[4], sqrt(a2[4] - a[4]*a[4]) * 1000.);   
    Serial.printf("                V5 = %8.5f V DC (%7.2f mV)\n",
                  a[5], sqrt(a2[5] - a[5]*a[5]) * 1000.);   
    Serial.printf("          Diff 3/4 = %8.5f V DC (%7.2f mV)\n",
                  a[6], sqrt(a2[6] - a[6]*a[6]) * 1000.);   
    Serial.printf("  Resistance 5/GND = %8.0f Ohms (for 10K ref)\n",
                  a[5] / (vRef - a[5]) * 10000);  
    Serial.printf(" %5.1f C, %6.0f Pa, %5.1f %%RH, g = %5.3f, %5.3f, %5.3f\n", 
                  temperature, pressure, humidity, gx, gy, gz); 
    Serial.printf("\n");   
  }
  if (digitalRead(BUTTON_PIN)) {
    Serial.printf("%8.3f", t);
    for(int i = 0;i < 7;i++) Serial.printf(", %6.3f", a[i]);   
    Serial.printf(", %8.0f", a[5] / (vRef - a[5]) * 10000);  
    Serial.printf(", %4.1f, %6.0f, %5.1f,   %.2f, %.2f, %.2f", 
                  temperature, pressure, humidity, gx, gy, gz); 
    Serial.printf("\n");   
  }
  timeLast = timeNow;      // save the old value for next time through the loop
  delay(1);
}
