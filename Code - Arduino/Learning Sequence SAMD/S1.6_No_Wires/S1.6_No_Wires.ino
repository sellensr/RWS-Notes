#define HI_PIN 2

void setup() {
  // put your setup code here, to run once:
  pinMode(HI_PIN,OUTPUT);
  digitalWrite(HI_PIN,HIGH);

  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A2,HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(millis()/1000.,3);
  Serial.print(", ");
  Serial.print(analogRead(A1));
  Serial.print(", ");
  Serial.print(analogRead(A5));
  Serial.print("\n");
}
