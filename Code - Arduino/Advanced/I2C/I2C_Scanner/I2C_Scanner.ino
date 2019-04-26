// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    https://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

// https://cdn-learn.adafruit.com/downloads/pdf/i2c-addresses.pdf
// provides a list of sensors that might go with those addresses

#include <Wire.h>


void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}

#define BMX_ADDRESS_A             (0x77)
#define BMX_ADDRESS_B             (0x76)
#define BMX_REGISTER_CHIPID       (0xD0)
#define BNO_ADDRESS_A             (0x28)
#define BNO_ADDRESS_B             (0x29)
#define BNO_REGISTER_CHIPID       (0x00)
#define BNO_CHIPID                (0xA0)

unsigned checkBMX(byte addr) {
  unsigned sensorID = read8(addr, BMX_REGISTER_CHIPID);
  if (sensorID == 0xFF) return 0;
  return sensorID;
}
unsigned checkBNO(byte addr) {
  unsigned sensorID = read8(addr, BNO_REGISTER_CHIPID);
  if (sensorID == 0xFF) return 0;
  return sensorID;
}

uint8_t read8(byte addr, byte reg) {
  uint8_t value;

  Wire.beginTransmission((uint8_t)addr);
  Wire.write((uint8_t)reg);
  Wire.endTransmission();
  Wire.requestFrom((uint8_t)addr, (byte)1);
  value = Wire.read();
  return value;
}

void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");
      if (address == BMX_ADDRESS_A || address == BMX_ADDRESS_B) {
        unsigned sensorID = checkBMX(address);
        if (sensorID) {
          Serial.print("Found a BMP/BME Sensor ID = 0x");
          Serial.print(sensorID, HEX);
          Serial.print(" at address 0x");
          Serial.println(address,HEX);
          Serial.println("BMP085/BMP180=0x55, BMP280=0x56-0x58, BME280=0x60, BME680=0x61\n");
        }
      }
      if (address == BNO_ADDRESS_A || address == BNO_ADDRESS_B) {
        unsigned sensorID = checkBNO(address);
        if (sensorID) {
          Serial.print("Found a BNO Sensor ID = 0x");
          Serial.print(sensorID, HEX);
          Serial.print(" at address 0x");
          Serial.println(address,HEX); Serial.println();
        }
      }
      if(address == 0x3C) Serial.println("Could be an Adafruit_SSD1306 128x32 OLED Display\n");
      if(address == 0x68) Serial.println("Could be an RTClib supported Real Time Clock\n");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");


  delay(5000);           // wait 5 seconds for next scan
}
