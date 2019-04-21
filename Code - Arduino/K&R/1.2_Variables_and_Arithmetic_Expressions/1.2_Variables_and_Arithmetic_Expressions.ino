// An Arduino Version of K&R Example 1.2
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

/* print Fahrenheit-Celsius table
    for fahr = 0, 20, ..., 300 */

void setup() {
  int fahr, celsius;
  int lower, upper, step;

  Serial.begin(115200);     // setup() calls the Serial library function begin()
  lower = 0;                /* lower limit of temperature table */
  upper = 300;              /* upper limit */
  step = 20;                /* step size */
  fahr = lower;
  while (fahr <= upper) {
    celsius = 5 * (fahr-32) / 9;
    Serial.print(fahr);     // Serial.print() is limited compared to printf()
    Serial.print("\t");
    Serial.print(celsius);
    Serial.print("\n");
    fahr = fahr + step;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
