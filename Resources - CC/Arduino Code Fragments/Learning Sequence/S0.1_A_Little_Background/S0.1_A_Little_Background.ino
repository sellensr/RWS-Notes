void my_main(){        // I can't call this main() because there is already a hidden main()
  setup();             // this call to setup() will happen once
  while(true) loop();  // this call to loop() will be repeated for ever
  int i = 5;           // this line will never be executed 
}

void setup() {  // a function called setup() that takes no arguments
                // and returns nothing "void"
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {   // a function called loop() that takes no arguments
                // and returns nothing "void"
  // put your main code here, to run repeatedly:
  int j = millis()/1000;
  int k = mySquare(j);
  Serial.print(j); 
  Serial.print(", ");
  Serial.print(k);
  Serial.print(", ");
  Serial.println(sqrt(k));
  delay(200);
}

int mySquare(int i){  // a function called mySquare takes one int argument i
  return i*i;         // and returns an integer
}

