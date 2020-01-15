void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial port speed
  // Use Serial.print() for single element outputs, or for programming the UNO
  Serial.print("\n\nS1 Hello World\n\n");
  pinMode(12,INPUT_PULLUP);
  pinMode(13,OUTPUT);
  // Set the bit resolution for the values reported by analogRead().
  // Not supported on the UNO!
  analogReadResolution(16);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long t = micros();
  int d12 = digitalRead(12);
  unsigned a1 = analogRead(A1);
  // Be careful not to send too much to Serial all at once, the output buffer is small.
  Serial.printf("%9lu us since start, %d digitalRead on pin 12,", t, d12);
  // Use Serial.printf() for formatted output made up of multiple elements.
  // (not available on the UNO due to memory limitations!)
  Serial.printf(" %5u analogRead on A1, and Hello Again!\n", a1);
  if(digitalRead(13) == 1) digitalWrite(13,0);
  else digitalWrite(13,1);
  delay(500);
}
