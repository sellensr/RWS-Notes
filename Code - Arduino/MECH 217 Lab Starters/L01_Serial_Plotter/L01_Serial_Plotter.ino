// This code started with Learning Sequence S1, but has been expanded to make it 
// clearer some of the steps in typical microcontroller application.
// Revised to work better with the Itsy Bitsy M0
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.print("\n\nHello World, L01 Serial Plotter\n\ntime, button, photo, led\n");
  pinMode(12,INPUT_PULLUP);     // use pin 12 for input from the button
  pinMode(13,OUTPUT);           // use pin 13 for output to blink the onboard LED
}

void loop() {
  unsigned long timeNow = 0;  // [provide your own descriptions for what these lines do]
  boolean but = LOW;          // 
  unsigned pht = 0;           // 
  boolean led = 0;            // 
  
  // Then get new values for all of the quantities before you process them.
  timeNow = micros();
  but = digitalRead(12);      // 
  pht = analogRead(A3);       // 
  led = digitalRead(13);      //

  int ibut = 100 + but * 100;      //
  int iled = 300 + led * 200;      //

  // Finally, take action on the results!
  // In this case we are just blinking the onboard LED on and off
  if(digitalRead(13) == HIGH) digitalWrite(13,LOW);   // if on, then turn off
  else digitalWrite(13,HIGH);                         // otherwise turn on

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
