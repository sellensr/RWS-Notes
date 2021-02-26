#include "RTClib.h"   // RTClib by Adafruit needed for %f in printf()
#include <Adafruit_DotStar.h>
Adafruit_DotStar m0dotStar(1, 41, 40, DOTSTAR_BRG);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);              // set the serial port speed
  while(!Serial && millis() < 5000); // wait for the USB to wake up
  // Use Serial.print() for single element outputs, or for programming the UNO
  Serial.printf("\n\nRick's M0 Setup for S1\n\n"); 
  Serial.printf("\nBe careful when using Serial.printf() because there is a limited buffer that is quite short\n");
  Serial.print("\nBe careful when using Serial.printf() because there is a limited buffer that is quite short\n so you should use Serial.print() or several Serial.printf() calls in a row\n\n");

  // Turn off the bright LED to make the pin labels readable without glare
  m0dotStar.setBrightness(0);
  m0dotStar.show();   

  // Set the resolutions so that input is 16 bit, going from 0-65535
  analogReadResolution(16);

  // Make A2 a ground pin so you can just plug in the pot on A0/A1/A2
  pinMode(A2,OUTPUT);
  digitalWrite(A2,0);
  // Start with A0 at +3.3V for the pot
  analogWrite(A0,65535);

  // put a pushbutton across pins 10 and 12
  pinMode(10, OUTPUT);        // ground for the pushbutton
  digitalWrite(10, 0);
  pinMode(12, INPUT_PULLUP);  // for reading the pushbutton state

  // For LED output on just about every Arduino-like board
  pinMode(13, OUTPUT);

  Serial.printf("Time [s], Pin 12, A3\n");  // provide headings to name the outputs in loop()
}

void loop() {
  // put your main code here, to run repeatedly:
  float t = micros() / 1000000.;// time in seconds, decimal point!
  int d12 = digitalRead(12);    // pushbutton state from pin 12
  int a3 = analogRead(A3);      // analog value from pin A3
  // Be careful not to send too much to Serial all at once, the output buffer is small.
  Serial.printf("%8.3f s since start, %d digitalRead() on pin 12,", t, d12);
  // Use Serial.printf() for formatted output made up of multiple elements.
  // (not available on the UNO due to memory limitations!)
  Serial.printf(" %5d\n", a3);
  digitalWrite(13,!digitalRead(13));// turn the LED on and off
  delay(1000);                      // delay makes things slow to respond
}
