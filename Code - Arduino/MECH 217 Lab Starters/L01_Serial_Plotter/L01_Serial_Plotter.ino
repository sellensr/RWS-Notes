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
  unsigned long timeNow = 0;  // [provide your own descriptions for what these lines do]
  boolean but = LOW;          // 
  unsigned pht = 0;           // 
  boolean led = 0;            // 
  
  // Then get new values for all of the quantities before you process them.
  timeNow = micros();
  but = digitalRead(BUTTON_PIN);  //
  pht = analogRead(CDS_PIN);      //
  led = digitalRead(LED_PIN);     //

  int ibut = 100 + but * 100;      //
  int iled = 300 + led * 200;      //

  // Finally, take action on the results!
  // In this case we are just blinking the onboard LED on and off
  if(digitalRead(LED_PIN) == HIGH) digitalWrite(LED_PIN,LOW);   // if on, then turn off
  else digitalWrite(LED_PIN,HIGH);                              // otherwise turn on

  // Then print out status information or update displays -- maybe not every time
  // Printing to a serial port or display could be the slowest thing in your loop!
  Serial.print(timeNow/1000000.);  // Uncomment these two lines to include the timestamp.
  Serial.print(", ");
  Serial.print(ibut);
  Serial.print(", ");
  Serial.print(pht);
  Serial.print(", ");
  Serial.print(iled);
  Serial.print("\n");
  delay(50);   // DON'T USE delay() except to slow things down for observation!
}
