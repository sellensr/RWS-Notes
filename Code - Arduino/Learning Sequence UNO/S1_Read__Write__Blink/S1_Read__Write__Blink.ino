void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial port speed
  Serial.print("\n\nS1 Hello World\n\n");
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(micros());
  Serial.print(" us since start, ");
  Serial.print(digitalRead(10));
  Serial.print(" digitalRead on pin 10, ");
  Serial.print(analogRead(A0));
  Serial.print(" analogRead on A0, and Hello Again!");
  Serial.print("\n");
  if(digitalRead(13) == 1) digitalWrite(13,0);
  else digitalWrite(13,1);
  delay(500);
}
