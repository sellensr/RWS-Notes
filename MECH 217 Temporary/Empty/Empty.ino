
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  Serial.print("\n\nEmpty Test\n\n");
  Serial.println(micros());
}

void loop() {
  // put your main code here, to run repeatedly:

}
