// Set the pins in af.h to match your processor wiring
// Make sure to connect the ADC_ACTIVE_PIN to the ADC_DETECT_PIN
#include "af.h"
// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;          // the time we started the current loop(), microseconds
float timeNowS = 0.0;               // timeNow, but in seconds, float so it will have decimals
unsigned long timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds
volatile unsigned buttonCount = 0;  // number of times the button has been pushed
volatile unsigned adCount = 0;      // number of analog to digital conversions made

// Interrupt Service Routine (ISR) gets called when an interrupt condition happens.
// This ISR simply counts the number of events, without taking any other action.
// https://learn.adafruit.com/make-it-switch/debouncing 
void bp(){  // ISR for a pushed button, will over count if button bounces
  buttonCount++;
}
// This ISR counts each event and also toggles the LED_PIN
void ad(){  // ISR for an ADC event, count should be accurate and not bounce
  adCount++;
  if(digitalRead(LED_PIN) == HIGH) digitalWrite(LED_PIN,LOW);
  else digitalWrite(LED_PIN,HIGH);
}

void setup() {
  Serial.begin(115200);              // set the serial port speed
  while(!Serial && millis() < 5000); // wait for the USB to wake up

  // Put a potentiometer on pins A0/A1/A2 to provide analog input on A1
  pinMode(A0,OUTPUT);                     // Set A0 and A2 to be the ends of the voltage divider
  digitalWrite(A0,HIGH);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);
  analogReadResolution(ANALOG_RES);

  // Setup the IO and Interrupt pins
  pinMode(LED_PIN,OUTPUT);              // Lights the onboard LED  
  pinMode(BUTTON_PIN, INPUT_PULLUP);    // Button pulls low when pushed
  pinMode(ADC_ACTIVE_PIN, OUTPUT);      // Pull low while converting
  pinMode(ADC_DETECT_PIN, INPUT);       // Read the state of the active pin
  // attachInterrupt(0,bp,FALLING);     // using the interrupt number directly can cause portability problems
  // Use digitalPinToInterrupt to do the conversion from pin number to interrupt number
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),bp,FALLING);    // Falls on push
  attachInterrupt(digitalPinToInterrupt(ADC_DETECT_PIN),ad,RISING); // Rises on completion

  Serial.printf("\n\nCounting Buttons and Conversions\n\n");
  Serial.printf("Time [s], A1 Mean, A1 RMS, Button Pushes, "); // provide headings to name the outputs
  Serial.printf("Conversions Counted in Loop, By Interrupt\n"); // provide headings to name the outputs
}

void loop() {
  static unsigned long lastPrint = 0;     // the time we last printed the results
  static double sum = 0., sum2 = 0.;
  static unsigned n = 0;
  timeNow = micros();                     // the time we started this loop, microseconds
  timeNowS = timeNow / 1000000.;          // time now in seconds, float so it will have decimals

  delay(random(85,147));                  // delay a random length of time
  digitalWrite(ADC_ACTIVE_PIN, LOW);      // pull low to show ADC happening
  double aValue = analogRead(A1);         // read an analog value and accumulate results
  sum += aValue;
  sum2 += aValue*aValue;
  n++;                                    // keep count of how often ADC happens
  digitalWrite(ADC_ACTIVE_PIN, HIGH);     // ADC sequence done

  if(timeNow - lastPrint >= 5000000 && n > 4){
    lastPrint = timeNow;
    unsigned bc = buttonCount;          // retrieve and then reset the counts right away
    unsigned ac = adCount;             
    buttonCount = 0;
    adCount = 0;
    Serial.printf("%8.2f, %8.2f, %4u, %4u, %4u\n", sum/n, // the mean value
                  sqrt(sum2-sum*sum/n)/(n-1), // the RMS value ~ standard deviation
                  bc,     // button pushes counted by the interrupt
                  n, ac); // ADC counted in the loop should match counted by the interrupt
    sum = sum2 = n = 0;                 // reset the count and the sums
  }
  timeLast = timeNow;      // save the old value for next time through the loop
}
