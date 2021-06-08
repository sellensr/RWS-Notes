void setup() {
  Serial.begin(115200);   // the serial monitor on the USB -- type in the top bar
  Serial1.begin(115200);  // the other serial port on the RX / TX pins of the board
  while(!Serial && millis() < 10000); // Wait for the USB to wake up
  Serial.print("\n\nS2.4_SerialLoop\n\n");
  Serial.print("Connect RX and TX pins on a single board and it will talk to itself.\n");
  Serial.print("(Type your input into the top line of the serial monitor and hit return.)");
  Serial.print("Connect two boards RX to TX from one to the other in both directions,\n");
  Serial.print("and they will talk to each other. Try running one of them on a phone supply.\n\n");
  pinMode(13,OUTPUT);
}

void loop() {
  // Change the RWS to your own initials so you can tell which machine you are looking at
  Serial.print("\nReading from Serial monitor on RWS [ASCII]:***");
  digitalWrite(13,HIGH);        // turn on the LED while reading
  while(Serial.available()){    // as long as there is a character waiting on Serial input
    char c = Serial.read();     // read a single character
    Serial1.write(c);           // write a single character to Serial1
    Serial.write(c);            // write the same to the serial monitor so we can see it
    Serial.print("[");          // write the ASCII character code as an integer in square brackets
    Serial.print((int) c); 
    Serial.print("]");
    delay(1000);   //slow down so we can see what's happening
  }
  Serial.print("*** and written to Serial1\n");
  digitalWrite(13,LOW);

  Serial.print("\nRead from Serial1 on RWS:***");
  while(Serial1.available()){
    char c = Serial1.read();
    Serial.write(c);  
    delay(1000);   //slow down so we can see what's happening
  }
  Serial.print("***\n");

  delay(2000); //slow down so we can see what's happening
}
