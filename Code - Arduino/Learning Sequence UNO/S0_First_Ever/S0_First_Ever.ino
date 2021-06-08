void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial port speed
  Serial.print("\n\nHello World\n\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(millis());
  Serial.print(" ms since start, Hello Again");
  Serial.print("\n");
  delay(500);   // try to avoid using delay()
}
