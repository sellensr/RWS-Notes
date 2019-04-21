// This code started with Learning Sequence S1, but has been expanded to make it 
// clearer some of the steps in typical microcontroller application.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         // set the serial port speed
  Serial.print("\n\nHello World, W01 Serial Plotter\n\n");
  pinMode(10,INPUT);            // use pin 10 for input from the button
  pinMode(13,OUTPUT);           // use pin 13 for output to blink the onboard LED
}

void loop() {
  unsigned long timeNow = 0;  // [provide your own descriptions for what these lines do]
  boolean but = LOW;          // 
  unsigned tmp = 0;           // 
  unsigned pht = 0;           // 
  boolean led = 0;            // 
  
  // Then get new values for all of the quantities before you process them.
  timeNow = micros();
  but = digitalRead(10);      // 
  tmp = analogRead(A2);       // 
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
  Serial.print(timeNow);  // Uncomment these two lines to include the timestamp.
  Serial.print(", ");
  Serial.print(ibut);
  Serial.print(", ");
  Serial.print(tmp);
  Serial.print(", ");
  Serial.print(pht);
  Serial.print(", ");
  Serial.print(iled);
  Serial.print("\n");
  delay(50);   // DON'T USE delay() except to slow things down for observation!
}
