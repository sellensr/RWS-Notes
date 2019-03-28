/*   Master Application Framework - SAMD version for more advanced boards like the Feather M0
 *    (the master version has all of the features and will work on the UNO, while the UNO 
 *      version requires fewer resources and has been stripped down to make it simpler to follow.)
 *    
 *   An Application Framework that does rudimentary multitasking with
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
 *   If a button is wired to BUTTON_PIN, pushing the button will switch between
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
#include <Wire.h>     // communication with I2C devices like the BME280, LIS3DH
#include <SPI.h>      // communication with SPI devices like the SD card

// define DEBUG as true to get verbose monitoring
#define DEBUG             0
#define VERSION           0.9

// Choices of interrupt capable pins are very limited on the UNO
// UNO -- Put a button to pull PIN 2 (Interrupt Capable), low when it is pushed
// On the M0 Adalogger, try pin 12, among other good choices
#if defined(ARDUINO_ARCH_SAMD)      // exists for SAMD boards like the M0
  #define BUTTON_PIN       12       // wire to be low when it is pushed
  #define ANALOG_BITS      16       // Analog conversion resolution, 16 bit will work for SAMD
  #define ANALOG_MAX    65535       // Maximum analogRead value = 65535 for 16 bit, padded if lower res
#elif defined (ESP32)
  #define BUTTON_PIN       12       // wire to be low when it is pushed
  #define ANALOG_BITS      12       // Analog conversion resolution, 12 bit fixed for ESP32
  #define ANALOG_MAX     4095       // Maximum analogRead value = 4095 for 12 bit ESP32               
#else                               // Otherwise treat it like an UNO
  #define BUTTON_PIN        2       // wire to be low when it is pushed
  #define ANALOG_BITS      10       // Analog conversion resolution for the UNO is fixed at 10 bits
  #define ANALOG_MAX     1023       // Maximum analogRead value = 1023 for 10 bit UNO
#endif

// Memory is crowded to make all this work in the UNO's 2K of RAM!
// Just leave out the SD card code unless the target is a SAMD and save hundreds of bytes!
// Each channel in NCH requires more than 26 bytes in global memory. 
// Each task in MAX_TASKS requires more than 22 bytes in global memory.
#define MAX_TASKS        10       // Provide arrays large enough to manage this many tasks 
#define CONSOLE_TASK  MAX_TASKS-1 // Normally the last task in the list 
#define CONSOLE_TIME      1.0     // Report this often to the serial monitor [seconds]
#define SD_TASK       MAX_TASKS-2 // Normally the second last task in the list 
#define SD_TIME           1.0     // Report this often to the SD card log file [seconds]
#define NCH               6       // Collect analog data from this many channels or ports, A0 to AX, X=NCH-1
#define AREF              3.26    // Analog Reference Voltage measured for this microcontroller
#define NCHD             14       // Collect digital data from this many channels or pins, 0 to NCHD-1
#define BLINK_PIN        13       // Use the LED on the UNO itself for any blinking

// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println


// Global Variables are available throughout the code and don't lose their values like locals.
// These variables can share data between tasks and other functions without customizing arguments.
// Note that the timing resolution is 4 us on the UNO, but 1 us on some other processors.
// Using double for floating point times should maintain resolution, except on the UNO.
// UNO only does 32 bit floats, and doubles will be automatically reduced to floats.

// This group tracks timing for the overall operation of the loop() function
unsigned long   timeNow = 0;         // the time we started the current loop(), microseconds
double         timeNowS = 0.0;       // timeNow, but in seconds, float/double so it will have decimals
unsigned long  timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds
unsigned long        dt = 0;         // timeNow - timeLast in microseconds
double              dtS = 0.0;       // the same time difference in seconds
double         loopTime = 0.0;       // a smoothed value of dtS -- gives an idea of speed
unsigned long loopCount = 0;         // the number of loop() calls completed so far
float         skipTimeS = 0.02;      // skip remaining tasks if a loop has already run this long

// This group tracks timing and status for the individual task functions called by loop()
int (*fTask[MAX_TASKS])(int) = {0};  // pointers to the functions that do each task
volatile double dtTaskMin[MAX_TASKS];// the minimum number of seconds between calls, could change in interrupts
unsigned long lastTask[MAX_TASKS];   // last time a loop() started that called this task
unsigned long lastTaskRT[MAX_TASKS]; // the number of microseconds it took to run last time
unsigned long worstTaskRT[MAX_TASKS];// the largest number of microseconds it took to run
int lastTaskReturn[MAX_TASKS];       // the return last time a task was called

// This group holds actual data and results produced by the various tasks
bool    d[NCHD] = {0};               // the latest digitalRead() values [-]
unsigned a[NCH] = {0};               // the latest analogRead() values [-]
float  tau[NCH] = {0};               // time constants for smoothing may be different for each channel [s]
float   as[NCH] = {0};               // the latest smoothed analogRead() values [-]
float    v[NCH] = {0};               // the latest voltages [V]
float   vs[NCH] = {0};               // the latest smoothed voltages [V]
float    x[NCH] = {0};               // the latest temperatures or whatever [app specific]
float   xs[NCH] = {0};               // the latest smoothed temperatures or whatever
float      pres = NAN,               // environmental values typically from a BME280, pressure [Pa]
           temp = NAN,               // temperature [C]
            hum = NAN;               // relative humidity [%]
