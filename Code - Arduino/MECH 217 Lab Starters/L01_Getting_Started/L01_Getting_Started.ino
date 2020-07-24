// This code started with Learning Sequence S1, but has been expanded to make it 
// clearer some of the steps in typical microcontroller application.
// Revised to work better with the Itsy Bitsy M0 for 2020
#define LED_PIN 13      // built in LED
#define BUTTON_PIN 12   // pushbutton conection pin
#define HI_PIN 2        // pin 2 needs to be high for the CdS circuit
#define CDS_PIN A5      // A5 is the middle of the CdS circuit
#define vRef 3.300      // This value will depend on your own hardware

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);               // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.print("\n\nHello World, L01 Starter\n\n");
  pinMode(BUTTON_PIN,INPUT_PULLUP);   // use built in pullup resistor for input from the button
  pinMode(LED_PIN,OUTPUT);            // use LED_PIN for output to blink the onboard LED
  pinMode(HI_PIN,OUTPUT);             // set the output high (near vRef)
  digitalWrite(HI_PIN,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  // First declare and initialize all variables, with descriptive comments.
  // You could declare them on the fly, while getting the new values,
  // but do it this way until you can reliably avoid mistakes.
  unsigned long timeNow = 0;  // the time this round of the loop started in us
  boolean but = LOW;          // the digital value returned by the button
  unsigned pht = 0;           // the unsigned integer value from the CdS photocell ADC
  boolean led = 0;            // the digital value currently on pin 13
  
  // Then get new values for all of the quantities before you process them.
  timeNow = micros();
  but = digitalRead(BUTTON_PIN);
  pht = analogRead(CDS_PIN);
  led = digitalRead(LED_PIN); 

  // Then process the new values to update your estimate of the state of the world
  // This is where you might do some smoothing or averaging, or convert ADC values to
  // voltage or temperature or something else.
  // NO PROCESSING IN THIS EXAMPLE, so this is just a place holder.

  // Finally, take action on the results!
  // In this case we are just blinking the onboard LED on and off
  if(digitalRead(LED_PIN) == HIGH) digitalWrite(LED_PIN,LOW);   // if on, then turn off
  else digitalWrite(LED_PIN,HIGH);                              // otherwise turn on

  // Then print out status information or update displays -- maybe not every time
  // Printing to a serial port or display could be the slowest thing in your loop!
  Serial.print(timeNow / 1000000.);
  Serial.print(" s since start, ");
  Serial.print(but);
  Serial.print(" digitalRead on BUTTON_PIN, ");
  Serial.print(pht);
  Serial.print(" analogRead from photocell, ");
  Serial.print("\n");
  delay(500);   // DON'T USE delay() except to slow things down for observation!
}
