// This code is to use as an example before L01
// build in sequence

#define BUTTON_PIN 12                                     // 3.0c

void setup() {
  Serial.begin(115200);                 // 1.2
//  delay(5000);                        // 1.5a
//  while(!Serial);                     // 1.5b
  while(!Serial && millis() < 5000);    // 1.5c
  Serial.print("\n\nHello World\n\n");  // 1.0
  
  pinMode(BUTTON_PIN,INPUT_PULLUP);                       // 3.3
  
  pinMode(13,OUTPUT);                                                     // 5.0
  
  analogReadResolution(12);                                       // 4.5
  pinMode(2,OUTPUT);                                              // 4.6
  digitalWrite(2,HIGH);                                           // 4.6
}

void loop() {
  unsigned long timeNow = 0;                      // 2.1
  timeNow = micros();                             // 2.0
  
//  but = digitalRead(12);                                // 3.0a
//  boolean but = digitalRead(12);                        // 3.0b
  boolean but = digitalRead(BUTTON_PIN);                  // 3.0c
  
  unsigned pht = analogRead(A5);                                  // 4.0
  boolean led = digitalRead(13);                                  // 4.1

  if(digitalRead(13) == HIGH)                                             // 5.0
    digitalWrite(13,LOW);                                                 // 5.0
  else digitalWrite(13,HIGH);                                             // 5.0
  
//  Serial.print(timeNow);                        // 2.2a
//  Serial.print(timeNow / 1000000);              // 2.2c
//  Serial.print(timeNow / 1000000.);             // 2.2d
  Serial.print(timeNow / 1000000., 3);            // 2.2e
  Serial.print(" s since start, ");               // 2.3
  
  Serial.print(but);                                      // 3.1
  Serial.print(" digitalRead on BUTTON_PIN, ");           // 3.2
  
  Serial.print(pht);                                              // 4.2
  Serial.print(" analogRead from photocell, ");                   // 4.3
  
  Serial.print("\n");                             // 2.2b
  delay(1234);                                    // 2.4
}
