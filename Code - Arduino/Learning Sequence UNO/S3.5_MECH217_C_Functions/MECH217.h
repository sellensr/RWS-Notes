#ifndef _MECH217C_h     // prevent the header from being included multiple times
#define _MECH217C_h
#define TIME_DRIFT_INFO   ///< includes sysUnsyncedTime from TimeLib
#include <Arduino.h>
#include <Wire.h>         // to support I2C scanning
#include <TimeLib.h>      // to manage time conversions
#include <RTClib.h>       // to get the DateTime struct and supporting functions

// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data

#define P Serial.print                          ///< P(thing) expands to Serial.print(thing)
#define PL Serial.println                       ///< PL(thing) expands to Serial.println(thing)
#define PCS Serial.print(", ");Serial.print     ///< PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCSL Serial.print(", ");Serial.println  ///< PCSL(thing) expands to Serial.print(", ");Serial.println(thing)

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;   // the time we started the current loop(), microseconds
float timeNowS = 0;          // timeNow, but in seconds
unsigned long timeLast = 0;  // the last time we went through the loop(), microseconds

// RWS Standard Pin Assignments for consistency across projects
#define HALL_PIN            11    ///< Hall Effect Sensor goes low when magnet present
#define BUTTON_PIN          12    ///< Hard Wired, pulled low when pressed, good choice because 12 doesn't do PWM on ItsyBitsy M0
#define LED_PIN             13    ///< On Feather Board, and just about all others
#define TMP36_PIN           A0    ///< Hard Wired if included, but TMP36 is being phased out of MECH 217
#define POT_LOW_PIN         A0    ///< Plug pot directly to A0-A2 for calibration testing
#define POT_PIN             A1    ///< Hard Wired to middle pin of pot
#define POT_HIGH_PIN        A2    ///< The third pin of a direct plugged pot
#define AMP_PIN             A2    ///< Hard Wired if included, connect amplifier output here
#define CDS_PIN             A3    ///< Hard Wired if included, good place for permanently mounted photocell input
#define THERMISTOR_PIN      A4    ///< Connect Thermistor to A4/A5, resistor from A4 to Vcc
#define THERMISTOR_LOW_PIN  A5    ///< Pull to ground for thermistor
#ifdef ADC_RESOLUTION
#define ANALOG_RES          ADC_RESOLUTION    ///< Use the defined value if ADC_RESOLUTION is defined (SAMD)
#else
#define ANALOG_RES          10    ///< Use 10 because it must be an UNO or similar
#endif
#define BAT_PIN             A7    ///< Battery monitoring for feathers
#define BAT_FACTOR        2.00    ///< voltage divider step down
#define KC_OFFSET       273.15    ///< K for 0 deg C conversion factor

#ifndef BUILTIN_SDCARD            // may already be defined (e.g. Teensy 3.5)
#define BUILTIN_SDCARD 10         ///< the Adafruit default if not already defined
#endif

#endif // _MECH217C_h
