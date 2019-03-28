// An Arduino Version of K&R Example 1.3
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

/* print Fahrenheit-Celsius table */

void setup() {
  int fahr;

  Serial.begin(115200);     // setup() calls the Serial library function begin()
  for (fahr = 0; fahr <= 300; fahr = fahr + 20){
    Serial.print(fahr);     
    Serial.print("\t");
    Serial.print((5.0/9.0)*(fahr-32));
    Serial.print("\n");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
