/* Using the Virtual MECH 217 Lab (direct copy of Virtual217-Master.ino)
 *  
 *  When you want to test some code, but don't want to set up all the circuitry 
 *  for an experiment, you can use the Virtual217 library. Before you run this code:
 *  
 *      Find the Virtual217-Master folder under RWS-Notes/Code-Arduino/Advanced.
 *      
 *      Copy it to your Arduino libraries folder (~Documents/Arduino/libraries or similar).
 *      
 *      Restart the Arduino IDE so it will know the library is there.
 *   
 *  The library gives your three new functions:
 *      analogReadV() works just like analogRead(), except with virtual circuit voltages.
 *          It also runs more slowly, about 2000 Hz on an M0, or 300 Hz on an UNO R3
 *      analogReferenceV() works just like analogReference(), assuming an UNO R3
 *      randnV() returns a normally distributed, pseudo-random floating point value
 *          from a distribution with a mean of 1.0 and a standard deviation of 1.0
 * 
 */
#include "Virtual217.h"

void setup() {
  Serial.begin(115200);
  Serial.print("\n\nVirtual217 -- fakes analog inputs to allow you to do some code testing\n\n"); 
  //analogReferenceV(INTERNAL);
  unsigned long t1 = micros();
  for(int i = 0;i < 10000;i++) analogReadV(A0);
  Serial.print("\n\nVirtual 217\n");
  Serial.print((micros()-t1)/1000000.); Serial.print(" seconds to take 10000 reads.\n\n");
}

void loop() {
  Serial.print(micros()/1000000.,4);
  for(int i = 0;i<6;i++){
    Serial.print(", ");
    Serial.print(analogReadV(A0+i));
  }
  Serial.print(", ");
  Serial.print(randnV()+10);
  Serial.print("\n");
}
