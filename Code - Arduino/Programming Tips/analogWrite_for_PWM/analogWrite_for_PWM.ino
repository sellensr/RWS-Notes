#include "RTClib.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial && millis() < 5000);
  Serial.print("\n\nPWM Example\n\n");
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  int bright = (millis()/100) % 256;
  Serial.printf("%d, %d\n", bright, digitalRead(13) * 255);
  analogWrite(13,bright);
  delay(10);

}
