#define HI_PIN 2      // pin 2 can be used as a high level reference
#define LOW_PIN 10    // pin 10 provides the ground for the pushbutton on pin 12
#define BUTTON_PIN 12 // the button

void setup() {
  // put your setup code here, to run once:
  pinMode(HI_PIN,OUTPUT);
  digitalWrite(HI_PIN,HIGH);
  pinMode(LOW_PIN,OUTPUT);
  digitalWrite(LOW_PIN,LOW);
  pinMode(BUTTON_PIN,INPUT_PULLUP);

  analogWrite(A0,1023);   // set the A0 output High
  pinMode(A2,OUTPUT);     // set A2 to ground so you can read the pot on A1
  digitalWrite(A2,HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(millis()/1000.,3);
  Serial.print(", ");
  Serial.print(digitalRead(12));
  Serial.print(", ");
  Serial.print(analogRead(A1));
  Serial.print(", ");
  Serial.print(analogRead(A3));
  Serial.print("\n");
}
