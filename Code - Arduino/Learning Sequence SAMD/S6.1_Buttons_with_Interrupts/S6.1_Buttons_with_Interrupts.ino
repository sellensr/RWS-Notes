// Set the pins in af.h to match your processor wiring
#include "af.h"
// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;          // the time we started the current loop(), microseconds
float timeNowS = 0.0;               // timeNow, but in seconds, float so it will have decimals
unsigned long timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds
unsigned buttonCount = 0;           // number of times the button has been pushed
unsigned adCount = 0;               // number of analog to digital conversions made

// Interrupt Service Routine (ISR) gets called when an interrupt condition happens.
// This ISR simply counts the number of events, without taking any other action.
void bp(){  // ISR for a pushed button, may over count if button bounces
  buttonCount++;
}
// This ISR copunts each event and also toggles the LED_PIN
void ad(){  // ISR for an ADC event, count should be accurate and not bounce
  adCount++;
  if(digitalRead(LED_PIN) == HIGH) digitalWrite(LED_PIN,LOW);
  else digitalWrite(LED_PIN,HIGH);
}

void setup() {
  Serial.begin(115200);                   // set the serial port speed
  while(!Serial);                         // wait for port to wake up

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

  P("\n\nCounting Buttons and Conversions\n\n");
  P("Time [s], A1 Mean, A1 RMS, Button Pushes, Conversions Counted in Loop, By Interrupt\n"); // provide headings to name the outputs
}

void loop() {
  static unsigned long lastPrint = 0;     // the time we last printed the results
  static double sum = 0., sum2 = 0.;
  static unsigned n = 0;
  timeNow = micros();                     // the time we started this loop, microseconds
  timeNowS = timeNow / 1000000.;          // time now in seconds, float so it will have decimals

  delay(random(85,147));                  // delay a random length of time
  digitalWrite(ADC_ACTIVE_PIN, LOW);
  double aValue = analogRead(A1);
  sum += aValue;
  sum2 += aValue*aValue;
  n++;
  digitalWrite(ADC_ACTIVE_PIN, HIGH);

  if(timeNow - lastPrint >= 1000000 && n > 4){
    lastPrint = timeNow;
    unsigned bc = buttonCount;          // retrieve and then reset the counts right away
    unsigned ac = adCount;             
    buttonCount = 0;
    adCount = 0;
    P(timeNowS);
    PCS(sum/n);                         // the mean value
    PCS(sqrt(sum2-sum*sum/n)/(n-1));    // the RMS value ~ standard deviation
    PCS(bc);
    PCS(n);                             // counted in the loop should match
    PCSL(ac);                           // counted by the interrupt
    sum = sum2 = n = 0;                 // reset the count and the sums
  }
  timeLast = timeNow;      // save the old value for next time through the loop
}
