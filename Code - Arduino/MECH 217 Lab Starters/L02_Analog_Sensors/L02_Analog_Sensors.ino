void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);         // set the serial port speed
  while(!Serial && millis() < 5000);
  Serial.print("\n\nHello World, L02 Starter\n\n");
  Serial.print("Time [S], but, tmp, pht, out, tmpV, phtV, outV, phtPC\n");
  pinMode(12,INPUT_PULLUP);     // use pin 12 for input from the button
  pinMode(13,OUTPUT);           // use pin 13 for output to blink the onboard LED
  pinMode(2,OUTPUT);
  digitalWrite(2,HIGH);
  analogReadResolution(16);
}

unsigned long timeLast = 0;  // the last time we went through the loop, microseconds

void loop() {
  // put your main code here, to run repeatedly:
  // First declare and initialize all variables, with descriptive comments.
  // You could declare them on the fly, while getting the new values,
  // but do it this way until you can reliably avoid mistakes.
  unsigned long timeNow = 0;  // the time this round of the loop started in us
  boolean but = LOW;          // the digital value returned by the button
  unsigned tmp = 0;           // the unsigned integer value from the TMP36 ADC
  unsigned pht = 0;           // the unsigned integer value from the CdS photocell ADC
  unsigned out = 0;           // the unsigned integer value from the analog Output
  boolean led = 0;            // the digital value currently on pin 13
  // I have defined a bunch of variables for you to use so we will all be on the same page
  // and debugging will be easier in the lab. Try to avoid new variables unless you need them!
  float aRef = 3.300;         // the analog reference -- set to measured supply voltage for DEFAULT
  float timeNowS = 0.0;       // the time this round of the loop started in seconds
  float tmpV = 0.0;           // the voltage value from the TMP36 ADC
  float tmpC = 0.0;           // the temperature in C value from the TMP36 ADC
  float phtV = 0.0;           // the voltage value from the CdS photocell ADC
  float phtPC = 0.0;          // the CdS photocell expressed as a 0-100 light percentage level
  float outV = 0.0;           // the voltage value from the potentiometer ADC
  
  // Then get new values for all of the quantities before you process them.
  timeNow = micros();
  but = digitalRead(12);      // the button should be connected to pin 12
  tmp = analogRead(A5);       // the thermistor connected to pin A5
  pht = analogRead(A3);       // the photocell output should be connected to pin A3
  out = analogRead(A4);       // the analog output on (A0) connected to input pin A4
  led = digitalRead(13); 

  // Then process the new values to update your estimate of the state of the world
  // This is where you might do some SMOOTHING or averaging, or convert ADC values to
  // voltage or temperature or something else.
  timeNowS = timeNow / 1000000.0;   // make sure the division happens in floating point!
  // CALCULATE VALUES FOR THE VOLTAGES AND TEMPERATURE HERE




  
  // Finally, take action on the results!
  if(tmpC > 25.0) digitalWrite(13,HIGH);  // light the LED if temperature is too warm (turn on the AC)
  else digitalWrite(13,LOW);
  
  // Then print out status information or update displays -- maybe not every time
  // Printing to a serial port or display could be the slowest thing in your loop!
  // You could make this a lot simpler with the P, PCS, PCSL macros from Learning Sequence S1.1
  // or by using Serial.printf() if your microcontroller board supports it.
  Serial.print(timeNowS,3);
  Serial.print(", ");
  Serial.print(but);
  Serial.print(", ");
  Serial.print(tmp);
  Serial.print(", ");
  Serial.print(pht);
  Serial.print(", ");
  Serial.print(out);
  Serial.print(", ");
  Serial.print(tmpV,3);
  Serial.print(", ");
  Serial.print(phtV,3);
  Serial.print(", ");
  Serial.print(outV,3);
  Serial.print(", ");
  Serial.print(tmpC);
  Serial.print(", ");
  Serial.print(phtPC,1);
  Serial.print("\n");
  // CHANGE HERE to go as fast as you can if the button is pushed
  delay(500);   // DON'T USE delay() except to slow things down for observation!
  timeLast = timeNow;        // save the old value for next time through the loop
}
