// All this Serial.print() will get tedious -- define some shortcuts
// These will make it easy to generate CSV data
// P(thing) expands to Serial.print(thing)
#define P Serial.print
// PCS(thing) expands to Serial.print(", ");Serial.print(thing)
#define PCS Serial.print(", ");Serial.print
// PCSL(thing) expands to Serial.print(", ");Serial.println(thing)
#define PCSL Serial.print(", ");Serial.println

unsigned long timeLast = 0;  // the last time we went through the loop, microseconds

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);     // set the serial port speed
  P("\n\nRick's S1.2\n\n"); // worth printing out the name of the sketch to document
  P("Time [s], Pin 10, A0, Delta t [us]\n"); // provide headings to name the outputs
  pinMode(10,INPUT);
  pinMode(13,OUTPUT);
}

void loop() {
  // Keep time in microseconds as an unsigned long for best accuracy.
  unsigned long timeNow = micros();      // the time we started this loop, microseconds
  // unsigned long can hold 0,1,2,...,4294967295, about 4 billion, a little more than 71 minutes of us
  float timeNowS = timeNow / 1000000.;   // time now in seconds, float so it will have decimals
  // 32 bit floats have 6 or 7 significant figures and can reach +/- 3.4E38, so timeNowS is less precise 
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds
  // note that addition and subtraction wrap around for unsigned variables, so dt always works
  // 4294967295 + 1 = 0, 4294967295 + 2 = 1,...  0 - 4294967295 = 1, 3 - 4294967295 = 4, ...

  // get input and calculate values
  int d = digitalRead(10);            // 16 bit integer can hold -32768,...,-2,-1,0,1,2,..., 32767
  unsigned a = analogRead(A0);        // 16 bit unsigned can hold whole numbers 0,1,2,...,65535 
  byte led = digitalRead(13);         // 8 bit byte can hold whole numbers 0,1,2,...,255
  
  if(led == 1) digitalWrite(13,0);
  else digitalWrite(13,1);

  P(timeNowS,3);           // Print out current status, to 3 places usually precise enough
  PCS(d);
  PCS(a);
  PCSL(dt);

  timeLast = timeNow;      // save the old value for next time through the loop

  // The only time to use delay is to slow down a sketch so you can see what is going on
  delay(500);   // delay() is bad -- drone crashes happen in split seconds!
}
