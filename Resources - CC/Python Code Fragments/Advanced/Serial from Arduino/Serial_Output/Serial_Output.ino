void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial speed
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(millis()/1000.);
  Serial.print(", ");
  Serial.print(sin(millis()/1000.));
  Serial.print(", ");
  Serial.print(cos(millis()/1200.));
  Serial.print(", ");
  Serial.println(cos(millis()/1200.)*sin(millis()/1000.));
  delay(20);
}
