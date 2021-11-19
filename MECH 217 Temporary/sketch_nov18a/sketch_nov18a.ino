
byte iByte0 = 42;
byte iByte1 = 42;
unsigned long uLong0 = 42;
unsigned long uLong1 = 42;
unsigned long uLong2 = 42;
unsigned long timeNow = 27;
byte iByte2 = 42;

void setup() {
  // put your setup code here, to run once:
  unsigned long dt = micros();
  Serial.begin(115200);
  while(!Serial);
  Serial.print("\n\n test\n\n");
//  Serial.println(timeNow);
//  Serial.println(sizeof(timeNow));
  Serial.println(((unsigned) &timeNow) - 0x20000000,16);
  Serial.println(((unsigned) &dt) - 0x20000000);
  Serial.println(((unsigned) &iByte0) - 0x20000000,16);
  Serial.println(((unsigned) &iByte1) - 0x20000000,16);
  Serial.println(((unsigned) &iByte2) - 0x20000000,16);
  Serial.println(((unsigned) &uLong0) - 0x20000000,16);
  Serial.println(((unsigned) &uLong1) - 0x20000000,16);
  Serial.println(((unsigned) &uLong2) - 0x20000000,16);
}

void loop() {
  // put your main code here, to run repeatedly:

}
