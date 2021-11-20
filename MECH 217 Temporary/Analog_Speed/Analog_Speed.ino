void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial);
  double sum = 0, sum2 = 0;
  unsigned long start = micros();
  unsigned a = analogRead(A3);
  for(long i = 0; i < 100000L;i++){
    a = analogRead(A3);
    sum += a;
    sum2+=a*a;
  }
  Serial.println("\n\nTaking 100000 reads yields rate of:");
  Serial.print(1.0/((micros()-start)/1000000./100000.),4);
  Serial.print(" readings per second\n\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A3));
  delay(1000);
}
