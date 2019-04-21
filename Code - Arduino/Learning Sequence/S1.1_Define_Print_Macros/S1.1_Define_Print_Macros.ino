// All this Serial.print() will get tedious -- define some shortcuts
#define P Serial.print
#define PCS Serial.print(", ");Serial.print
#define PCSL Serial.print(", ");Serial.println

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial port speed
  P("\n\nHello World\n\n");
  P("Time [us], Pin 10, A0\n");
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  P(micros());
  PCS(digitalRead(10));
  PCSL(analogRead(A0));
  unsigned i = 5;
  int j = -12345;
  double x = 3.141592654;
  P("A format string for unsigned "); 
  P(i); 
  PCS("int "); 
  P(j); 
  PCS("and double "); 
  P(x, 3); 
  P("\n");
  if(digitalRead(13) == 1) digitalWrite(13,0);
  else digitalWrite(13,1);
  delay(500);
}
