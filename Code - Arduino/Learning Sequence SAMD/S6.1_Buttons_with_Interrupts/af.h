#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()

// Specify how to sample the analog inputs
#define NAVG           100    // Samples to Average Over
#define NCH              6    // Channels to Read
#define NCH_MAX          6    // Max possible Channels

// Common DIO pins include 9, 10, 11, 12, 13 on UNO, Feather, Itsy Bitsy
// The UNO only supports interrupts on pins 2 and 3, other boards more flexible
#define ADC_ACTIVE_PIN   9    // Pull low in code when getting data
#define ADC_DETECT_PIN  10    // Watch to see when analogRead() runs
#define BUTTON_PIN      12    // Hard Wired, pulled low when pressed
#define LED_PIN         13    // On Feather Board

// All boards include A0 - A5 analog inputs
#define TMP36_PIN       A0    // 14 Hard Wired
#define POT_PIN         A1    // 15 Hard Wired, could be position or servo, could use A0,A2 as positive and ground
#define AMP_PIN         A2    // 16 Hard Wired, thermocouple, load cell, PX137
#define CDS_PIN         A3    // 17 Hard Wired
#define THERMISTOR_PIN  A4    // 18 Connect with Jumper, 10K to positive for thermistor direct
#define OTHER_PIN       A5    // 19 Connect with Jumper, or ground for thermistor direct
// UNO doesn't support analogReadResolution() and is fixed at 10 bits 
#define ANALOG_RES      16    // Use the Max
#define KC_OFFSET      273.15 // K for 0 deg C

double analogMax = pow(2,ANALOG_RES) - 1;
double vRef = 3.29;
