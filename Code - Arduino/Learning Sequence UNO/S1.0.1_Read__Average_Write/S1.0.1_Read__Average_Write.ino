#define NAVG 10000

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial port speed
  Serial.print("\n\nS1.0.1 Hello World\n\n");
}

void loop() {
  double a = 0;
  for(int i = 0;i < NAVG;i++) a += analogRead(A5);
  a /= NAVG;
  Serial.print(micros()/1000000.);
  Serial.print(" s since start, ");
  Serial.print(a);
  Serial.print(" average analogRead()");
  Serial.print("\n");
  delay(1000);
}
