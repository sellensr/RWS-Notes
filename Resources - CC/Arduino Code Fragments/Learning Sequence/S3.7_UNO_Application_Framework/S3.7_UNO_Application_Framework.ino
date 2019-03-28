/*  An Application Framework that does rudimentary multitasking with
 *   real time targets for execution rates on tasks. The bare framework
 *   will click over at more than 1000 loops() per second, reading two 
 *   analog channels and looking after the overhead on an UNO. Doing nothing
 *   it will click over at about 2000 loops per second, so the overhead is 
 *   only requiring about 500 us per loop with MAX_TASKS set to 10. Reading 
 *   all six analog channels slows it down to 600 Hz, still quite fast!
 *   
 *   On the M0 Feather it maxes out over 1600 Hz and will do over 250 for all
 *   six analog channels. Note that the M0 samples slowly due to internal 
 *   averaging over multiple analog conversions
 *   
 *   If a button is wired to pin 2, pushing the button will switch between
 *   a slow display of the console data, and writing data out as fast as 
 *   possible. Writing data from all six channels at 115200 baud will 
 *   still get around the loop at over 100 Hz.
 *   
 *   There are no guarantees that tasks will always run on schedule.
 *   Each task will have the opportunity to run once per iteration through 
 *   the main loop, so individual tasks should all be designed to run quickly
 *   and give control back to the main loop.
 * 
 */
#include <Wire.h>

// define DEBUG as true to get verbose monitoring
#define DEBUG             0


// UNO -- Put a button to pull PIN 2 (Interrupt Capable), low when it is pushed
// Choices of interrupt capable pins are very limited on the UNO
// On the M0 Adalogger, try pin 12, among other good choices
#define BUTTON_PIN       12       // wire to be low when it is pushed
#define BLINK_PIN        13       // Use the LED on the UNO itself for any blinking
#define MAX_TASKS        10       // Provide arrays large enough to manage this many tasks 
#define CONSOLE_TASK  MAX_TASKS-1 // Normally the last task in the list 
#define CONSOLE_TIME      1.0     // Report this often to the serial monitor [seconds]
#define SD_TASK       MAX_TASKS-2 // Normally the second last task in the list 
#define SD_TIME           1.0     // Report this often to the SD card log file [seconds]
#define NCH               2       // Collect analog data from this many channels or ports, A0 to AX, X=NCH-1
#define AREF              3.26    // Analog Reference Voltage measured for this microcontroller

// Must be set to match the processor -- UNO does not support analogReadResolution()!
//#define ANALOG_BITS      10       // Analog conversion resolution for the UNO is 10 bits
//#define ANALOG_MAX     1023       // Maximum analogRead value = 1023 for 10 bit UNO
#define ANALOG_GT_10      1       // If defined, then analogReadResolution() call will be made
#define ANALOG_BITS      16       // Analog conversion resolution, 16 bit will work for all others
#define ANALOG_MAX    65535       // Maximum analogRead value = 65535 for 16 bit, padded if lower res

// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println


// Global Variables are available throughout the code and don't lose their values like locals.
// These variables can share data between tasks without customizing arguments.
unsigned long timeNow = 0;          // the time we started the current loop(), microseconds
float timeNowS = 0.0;               // timeNow, but in seconds, float so it will have decimals
float loopTime = 0.0;               // a smoothed value of dtS -- gives an idea of speed
unsigned long timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds
int (*fTask[MAX_TASKS])(int);       // pointers to the functions that do each task
volatile float dtTaskMin[MAX_TASKS];// the minimum number of seconds between calls, change in interrupts
unsigned long lastTask[MAX_TASKS];  // last time a loop() started that called this task
unsigned long lastTaskRunTime[MAX_TASKS];  // the number of microseconds it took to run last time
int lastTaskReturn[MAX_TASKS];      // the return last time a task was called

unsigned a[NCH];                    // the latest analogRead() values [-]
float tau[NCH];                     // time constants for smoothing may be different for each channel [s]
float as[NCH];                      // the latest smoothed analogRead() values [-]
float v[NCH];                       // the latest voltages [V]
float vs[NCH];                      // the latest smoothed voltages [V]
float x[NCH];                       // the latest temperatures or whatever [app specific]
float xs[NCH];                      // the latest smoothed temperatures or whatever
float pres = NAN,                   // environmental values typically from a BME280, pressure [Pa]
      temp = NAN,                   // temperature [C]
      hum = NAN;                    // relative humidity [%]
