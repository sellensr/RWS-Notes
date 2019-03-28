// An Arduino Version of K&R Example 1.4
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

#define   LOWER  0       /* lower limit of table */
#define   UPPER  300     /* upper limit */
#define   STEP   20      /* step size */

/* print Fahrenheit-Celsius table */

void setup() {
  int fahr;

  Serial.begin(115200);     // setup() calls the Serial library function begin()
  for (fahr = LOWER; fahr <= UPPER; fahr = fahr + STEP){
    Serial.print(fahr);     
    Serial.print("\t");
    Serial.print((5.0/9.0)*(fahr-32));
    Serial.print("\n");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
