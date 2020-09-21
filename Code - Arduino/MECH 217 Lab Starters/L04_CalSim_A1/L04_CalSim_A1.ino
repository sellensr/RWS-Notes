#include "RTClib.h"

#define LED_PIN 13      // built in LED
#define BUTTON_PIN 12   // pushbutton conection pin
#define HI_PIN 2        // pin 2 needs to be high for the CdS circuit
double  vRef = 3.300;      // This value will depend on your own hardware

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);               // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.print("\n\nHello World, L04 Scan A1\n\n");
  Serial.print("Put the pot in pins A0-A2 and adjust to different values.\n");
  Serial.print("Press the pushbutton to generate a simulated multimeter\n");
  Serial.print("measurement and a sequence of analogRead() values.\n\n");
  pinMode(BUTTON_PIN,INPUT_PULLUP);   // use built in pullup resistor for input from the button
  pinMode(LED_PIN,OUTPUT);            // use LED_PIN for output to blink the onboard LED
  pinMode(HI_PIN,OUTPUT);             // set the output high (near vRef)
  digitalWrite(HI_PIN,HIGH);
  analogReadResolution(16);
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  digitalWrite(A0,LOW);
  digitalWrite(A2,HIGH);

  // Uncomment the next two lines to switch to the 1.65 volt input scale
  // analogReference(AR_INTERNAL1V65);
  // vRef = 1.65;
}

void loop() {
  double timeNowS = micros()/1000000.;
  boolean but = digitalRead(BUTTON_PIN);
  if(!but){
    unsigned a = analogRead(A1);
    int n = micros()%300 - 150;
    double v = (a + n) / 19860. * vRef/3.3;
    Serial.printf("\n%8.3f s, Multimeter Reads %6.3f V\nData: (check %d,%d)\n", 
            timeNowS, v, a, n);
    for(int i = 0;i < 20;i++){
      Serial.println(analogRead(A1));
    }
    
    delay(1500);
  }
  
}
