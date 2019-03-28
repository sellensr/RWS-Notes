/*  You can include these functions in any sketch by copying and 
 *  pasting this sketch directory to your libraries folder.
 *  (usually Documents/Arduino/libraries or similar)
 *  
 *  If you use a sketch folder like this to design and test your code
 *  then you can fix bugs here, move it to the library, and have the 
 *  latest version available to all the sketches that use it.
 *  
 *  This is a compromise between having all your code in a single sketch
 *  and making the effort to build a full function C++ library, a much
 *  more complex coding task.
 *  
 *  S3.5.1 shows how another sketch can include this code from the 
 *  libraries folder
 * 
 */
 
#include "MECH217C.h"
#include "MECH217C.c"

void setup() {
  Serial.begin(115200);
}
  
void loop() {
  startLoop();
  static unsigned long cnt = 0;
  float v = AtoV(cnt,5.026);
  float T = VtoTMP36(v);
  P(timeNow);
  PCS(timeNowS,4);
  PCS(cnt);
  PCS(v,4);
  PCS(T,4);
  PCS(stepTS(timeNowS,2));
  PCS(sqPulseTS(timeNowS,8,4));
  PCS(sinPulseTS(timeNowS,4,8));
  PCS(normPulseTS(timeNowS,8,2));
  P("\n");
  delay(500);
  cnt++;
}
