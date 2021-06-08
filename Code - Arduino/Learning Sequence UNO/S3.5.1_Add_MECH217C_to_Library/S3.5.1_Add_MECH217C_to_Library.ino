/* Try compiling this code and see the error that results.
 *  
 *  To include files that aren't present in the sketch folder, the 
 *  IDE needs to be able to find them.
 *  
 *  Copy the whole folder S3.5_MECH217_C_Functions and paste it
 *  in your libraries folder (/Documents/Arduino/libraries or similar).
 *  
 *  Shut down the Arduino IDE and restart, then try compiling the code again.
 *  
 */
 
#include "MECH217.h"
#include "MECH217.c"

void setup() {
  Serial.begin(115200);
}
  
void loop() {
  startLoop();
  static unsigned long cnt = 0;
  float v = AtoV(cnt,5.026);
  P(timeNow/1000000.,3);
  PCS(cnt);
  PCS(v,4);
  P("\n");
  delay(500);
  cnt++;
}
