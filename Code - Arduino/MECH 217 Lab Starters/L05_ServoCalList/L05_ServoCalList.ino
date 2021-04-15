// 2021-04-14 Modified to work better with M0 processors 
#include <Servo.h>
uint8_t servoPin = 9;     // make sure these match your wiring
uint8_t feedbackPin = A3;
uint8_t ledPin = 13;
unsigned long timeLast = 0;

Servo myservo;            // create servo object to control a servo

int angles[] = {0,1,2,3,5,10,15,20,30,40,50,60,70,80,90,100,110,120,130,140,150,160,165,170,175,178,179,180,999};
float aValues[30] = {0};
float azero = 0, a180 = 0;

double goAngle(int a, unsigned wait = 0) {
  myservo.attach(servoPin);       // bring the servo under control of the microcontroller
  myservo.write(a);               // send it to the current angle
  delay(wait);                    // wait long enough for it to get there
  myservo.detach();               // stop driving the servo
  double val = 0;                 // get an average reading from the potentiometer
  for (int j = 0; j < 100; j++){
    val += analogRead(feedbackPin);
  }
  val /= 100;
  return val;
}

double getAngle(int n = 1) {
  double val = 0;                 // get an average reading from the potentiometer
  for (int j = 0; j < n; j++){
    val += analogRead(feedbackPin);
  }
  val /= n;
  val = (val - azero) / (a180-azero) * 180.;  // convert to an angle
  return val;
}

void setup() {
  Serial.begin(115200);
  while(!Serial && micros() < 10000000);
  analogReadResolution(16);
  Serial.print("\n\nServoCalList\n\nAverage analogRead() values for various angles: \n");

  // Start by getting some calibration values
  a180 = goAngle(180,1000);
  azero = goAngle(0,1000);
  for(int i = 0;  angles[i] <= 180; i++){ 
    aValues[i] = goAngle(angles[i],1000);
    Serial.print(angles[i]); Serial.print(", "); Serial.println(aValues[i]);

  }
  myservo.attach(servoPin);       // bring the servo under control of the microcontroller
}




void loop() {
  // put some code here to calculate a desired position value
  static int pos = 0;
  if(millis() % 10000 < 5000) pos = 90;
  else pos = 90 + 40 * cos(millis()/500.);
  myservo.write(pos);
  // you probably want to read the feedback and print some diagnostic results here
  double a = getAngle();
  Serial.print(pos); Serial.print(", "); Serial.println(a);
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{   // a floating point version of the map() function
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
