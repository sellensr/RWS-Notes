#include <Servo.h>
uint8_t servoPin = 9;     // make sure these match your wiring
uint8_t feedbackPin = A3;
uint8_t ledPin = 13;

Servo myservo;            // create servo object to control a servo

float aZero, a180;        // the analog read values corresponding to zero and 180 degrees

void setup() {
  Serial.begin(115200);
  while(!Serial && micros() < 10000);
  analogReadResolution(16);

  // Start by getting some calibration values
  
  myservo.attach(servoPin);       // bring the servo under UNO control
  myservo.write(0); delay(6000);  // send it to zero and wait long enough
  myservo.detach();               // stop driving the servo
  aZero = 0;                      // get an average reading from the potentiometer
  for (int i = 0; i < 100; i++) aZero += analogRead(feedbackPin);
  aZero /= 100;

  myservo.attach(servoPin);       // do the same for 180 degrees
  myservo.write(180); delay(6000); 
  myservo.detach(); 
  a180 = 0;
  for (int i = 0; i < 100; i++) a180 += analogRead(feedbackPin);
  a180 /= 100;

  Serial.print("\n\nServoCalStarter\n\nAverage analogRead() values for 0 and 180 degrees: ");
  Serial.print(aZero); Serial.print(", "); Serial.print(a180); Serial.print("\n\n");

  myservo.attach(servoPin);     // attaches the servo to the servo object
  myservo.write(0);             // go to zero degrees
  delay(3000);                  // wait to make sure it gets there

  ramp(100);                    // collect some slow moving ramp data
  step(20);                     // check for step response
}



void loop() {
  //  ramp(micros()/10000000);
  // put some code here to calculate a desired position value
  int pos = 0;
  if(millis() % 10000 < 5000) pos = 90;
  else pos = 90 + 40 * cos(millis()/500.);
  myservo.write(pos);
  // you probably want to read the feedback and print some diagnostic results here
}





void ramp(int wait) {   // run through the different postions in both directions
  Serial.print("\n\nIn ramp() with wait = "); Serial.print(wait);
  Serial.print("\nTime [s], set pos [deg], analog [-], pot [deg], pot - pos [deg]\n\n");
  for (int tpos = 0; tpos <= 360; tpos += 1) { // goes from 0 degrees to 180 degrees and back
    int pos = tpos;
    if (pos > 180) pos = 360 - pos;
    myservo.write(pos);                 // tell servo to go to position in variable 'pos'
    delay(wait);                          // waits for the servo to reach the position
    if (pos % 180 == 0 ) delay(1000);
    float a = analogRead(feedbackPin);
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

void step(int wait) {   // How fast does it respond to a step change?
  Serial.print("\n\nIn step() with wait = "); Serial.print(wait);
  Serial.print("\nTime [s], set pos [deg], analog [-], pot [deg], pot - pos [deg]\n\n");
  unsigned long tStart = millis();
  unsigned long tNow = millis();
  int pos = 0;
  while(tNow - tStart < 4000){
    tNow = millis();
    if(tNow - tStart < 2000) pos = 30; // tell servo to go to position
    else pos = 150;
    myservo.write(pos);
    delay(wait);                          // waits for the servo to reach the position
    float a = analogRead(feedbackPin);
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
