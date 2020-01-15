#include <Servo.h>
Servo myservo;  // create servo object to control a servo


float aZero, a180, sens;

void setup() {
  Serial.begin(115200);
//  analogReference(EXTERNAL);      // choose an analog reference that works for output voltage

  // Start by getting some calibration values
  
  myservo.attach(9);              // bring the servo under UNO control
  myservo.write(0); delay(6000);  // send it to zero and wait long enough
  myservo.detach();               // stop driving the servo
  aZero = 0;                      // get an average reading from the potentiometer
  for (int i = 0; i < 100; i++) aZero += analogRead(A0);
  aZero /= 100;

  myservo.attach(9);             // do the same for 180 degrees
  myservo.write(180); delay(6000); 
  myservo.detach(); 
  a180 = 0;
  for (int i = 0; i < 100; i++) a180 += analogRead(A0);
  a180 /= 100;

  Serial.print("\n\nServoCalStarter\n\nAverage analogRead() values for 0 and 180 degrees: ");
  Serial.print(aZero); Serial.print(", "); Serial.print(a180); Serial.print("\n\n");

  myservo.attach(9);            // attaches the servo on pin 9 to the servo object
  myservo.write(0);             // go to zero degrees
  delay(3000);                  // wait to make sure it gets there

  ramp(100);                   // collect some slow moving data
}

void loop() {
//  ramp(micros()/10000000);
}

void ramp(int wait) {
  Serial.print("\n\nIn ramp() with wait = "); Serial.print(wait);
  Serial.print("\nTime [s], set pos [deg], analog [-], pot [deg], pot - pos [deg]\n\n");
  for (int tpos = 0; tpos <= 360; tpos += 1) { // goes from 0 degrees to 180 degrees and back
    int pos = tpos;
    if (pos > 180) pos = 360 - pos;
    myservo.write(pos);                 // tell servo to go to position in variable 'pos'
    delay(wait);                          // waits for the servo to reach the position
    if (pos % 180 == 0 ) delay(1000);
    float a = analogRead(A0);
    float d = (a - aZero) * 180 / (a180-aZero);
    d = fmap(a,aZero,a180,0,180);
    Serial.print(micros() / 1000000., 4); Serial.print(", ");
    Serial.print(pos); Serial.print(", ");
    Serial.print(a); Serial.print(", ");
    Serial.print(d); Serial.print(", ");
    Serial.print(d-pos); Serial.print(", ");
    Serial.print("\n");
  }
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{   // a floating point version of the map() function
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
