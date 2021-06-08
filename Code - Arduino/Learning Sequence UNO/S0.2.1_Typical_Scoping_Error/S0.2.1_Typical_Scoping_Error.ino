void setup() {
  Serial.begin(115200);
}


void loop() {
  for(int i = 0;i<5;i++){
    int a = analogRead(A0); 
  }
  Serial.println(a);
}
