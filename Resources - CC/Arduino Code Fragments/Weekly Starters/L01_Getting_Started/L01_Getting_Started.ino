// This code started with Learning Sequence S1, but has been expanded to make it 
// clearer some of the steps in typical microcontroller application.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         // set the serial port speed
  Serial.print("\n\nHello World, W01 Starter\n\n");
  pinMode(10,INPUT);            // use pin 10 for input from the button
  pinMode(13,OUTPUT);           // use pin 13 for output to blink the onboard LED
}

void loop() {
  // put your main code here, to run repeatedly:
  // First declare and initialize all variables, with descriptive comments.
  // You could declare them on the fly, while getting the new values,
  // but do it this way until you can reliably avoid mistakes.
  unsigned long timeNow = 0;  // the time this round of the loop started in us
  boolean but = LOW;          // the digital value returned by the button
  unsigned tmp = 0;           // the unsigned integer value from the TMP36 ADC
  unsigned pht = 0;           // the unsigned integer value from the CdS photocell ADC
  boolean led = 0;            // the digital value currently on pin 13
  
  // Then get new values for all of the quantities before you process them.
  timeNow = micros();
  but = digitalRead(10);      // the button should be connected to pin 10
  tmp = analogRead(A2);       // the TMP36 output should be cconnected to pin A2
  pht = analogRead(A3);       // the photocell output should be connected to pin A3
  led = digitalRead(13); 

  // Then process the new values to update your estimate of the state of the world
  // This is where you might do some smoothing or averaging, or convert ADC values to
  // voltage or temperature or something else.
  // NO PROCESSING IN THIS EXAMPLE, so this is just a place holder.

  // Finally, take action on the results!
  // In this case we are just blinking the onboard LED on and off
  if(digitalRead(13) == HIGH) digitalWrite(13,LOW);   // if on, then turn off
  else digitalWrite(13,HIGH);                         // otherwise turn on

  // Then print out status information or update displays -- maybe not every time
  // Printing to a serial port or display could be the slowest thing in your loop!
  Serial.print(timeNow);
  Serial.print(" us since start, ");
  Serial.print(but);
  Serial.print(" digitalRead on pin 10, ");
  Serial.print(tmp);
  Serial.print(" analogRead from TMP36, ");
  Serial.print(pht);
  Serial.print(" analogRead from photocell, ");
  Serial.print("\n");
  delay(500);   // DON'T USE delay() except to slow things down for observation!
}
