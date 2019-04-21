// An Arduino Version of K&R Example 1.7
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

/* test power function */
int power(int base, int n); // *declares* the function without defining it, not required
                            // in Arduino if the function is in the same sketch file.

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
  int i;
  for (i = 0; i < 10; ++i){
    Serial.print(i);       
    Serial.print(", ");
    Serial.print(power(2,i));       
    Serial.print(", ");
    Serial.println(power(3,i));
  }       
}

void loop() {               // Arduino has a built in continuous loop()
}

/* power:  raise base to n-th power; n >= 0 */
int power(int base, int n)  // *defines* the function
{
  int i, p;
  p = 1;
  for (i = 1; i <= n; ++i)
    p = p * base;
  return p;
}
