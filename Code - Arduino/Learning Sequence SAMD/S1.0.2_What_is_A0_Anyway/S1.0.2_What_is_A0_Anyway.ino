#include <RTClib.h>     // oddly, enables %f in Serial.printf()

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   // set the serial port speed
  while(!Serial && millis() < 5000);         // wait for the USB to wake up
  Serial.print("\n\nS1.0.2 What is A0 Anyway?\n\n");
  Serial.print("The original UNO has digital IO pins numbered 0-13\n");
  Serial.print("and six analog inputs A0 through A5.\n");
  Serial.print("Serial.println(A0) gives: "); Serial.println(A0);
  Serial.print("Serial.println(A1) gives: "); Serial.println(A1);
  Serial.print("Serial.println(A2) gives: "); Serial.println(A2);
  Serial.print("Serial.println(A3) gives: "); Serial.println(A3);
  Serial.print("Serial.println(A4) gives: "); Serial.println(A4);
  Serial.print("Serial.println(A5) gives: "); Serial.println(A5);
  Serial.print("A0 through A5 are just #defined as numbers 14 through 19.\n");
  Serial.print("We can pass them around and store them like integers!\n");
}

void loop() {
}
