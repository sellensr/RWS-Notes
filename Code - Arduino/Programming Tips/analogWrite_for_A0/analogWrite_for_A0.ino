#include "RTClib.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial && millis() < 5000);
  Serial.print("\n\nanalogWrite() to analogRead Example\n\n");
  analogReadResolution(16);
}

void loop() {
  // put your main code here, to run repeatedly:
  double valSin = sin(millis() / 1000.) * 500 + 512;
  analogWrite(A0,valSin);
  Serial.printf("%8.3f, %d\n",valSin,analogRead(A3));
  delay(100);
}
