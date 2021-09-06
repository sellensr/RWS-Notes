#include "RTClib.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial && millis() < 5000);
  Serial.print("\n\nDigital GPIO Example\n\n");
  pinMode(10,OUTPUT);
  digitalWrite(10,LOW);
  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  delay(5000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("Pin 12 returns %d\n",digitalRead(12));
  delay(1000);

  if(digitalRead(13)){
    Serial.print("turning off\n"); 
    digitalWrite(13,LOW);
  }
  else{
    Serial.print("turning on\n"); 
    digitalWrite(13,HIGH);
  }

}
