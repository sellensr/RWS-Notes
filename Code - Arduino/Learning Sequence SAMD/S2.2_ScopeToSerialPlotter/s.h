// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PL(thing) expands to Serial.println(thing)
#define PL Serial.println
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeLast = 0;  // the last time we went through the loop, microseconds

#define BUTTON_PIN 12
