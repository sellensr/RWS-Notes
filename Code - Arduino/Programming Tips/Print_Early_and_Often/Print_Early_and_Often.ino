// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println

// Defining some more macros will make it a lot easier to print 
// out your results as you go along

// Debug versions will only print if DEBUG is non-zero
//#define DPCS if(DEBUG)Serial.print(", ");if(DEBUG)Serial.print
#define DP if(DEBUG) P
#define DPCS if(DEBUG) PCS
#define DPCSL if(DEBUG) PCSL

// define DEBUG as 0 for no DB printing, or define it as 1 or more to get printing
#define DEBUG 1

void setup() {
  Serial.begin(115200);
}

void loop() {
  float t = micros()/1000000; // this is going to do integer arithmetic!
  DP(t,4);                    // printing will let us see how the problem is generated!
  int i = analogRead(A0) * t; // this is going to break when t gets larger 
  DPCS(i);
  unsigned j = analogRead(A1);
  PCS(j);
  P("\n");
  delay(5000);
}

