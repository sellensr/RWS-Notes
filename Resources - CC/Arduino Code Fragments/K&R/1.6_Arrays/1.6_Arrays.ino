// An Arduino Version of K&R Example 1.6
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */
 /* count lines, words, and characters in input */

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
}

void loop() {               // Arduino has a built in continuous loop()
  int c, i, nwhite, nother;
  int ndigit[10];
 
  nwhite = nother = 0;
  for (i = 0; i < 10; ++i)
    ndigit[i] = 0;
  while ((c = Serial.read()) != -1){
    if (c >= '0' && c <= '9')
      ++ndigit[c-'0'];
    else if (c == ' ' || c == '\n' || c == '\t')
      ++nwhite;
    else
      ++nother;
    Serial.write(c);
    delay(5);
  }
  if (nwhite != 0){        // if there were white space characters counted
    Serial.print("\ndigits = ");
    for(i = 0; i < 10; ++i){
      Serial.print(ndigit[i]);
      Serial.print(", ");
    }
    Serial.print("white space = ");
    Serial.print(nwhite);       
    Serial.print(", other = ");
    Serial.println(nother);       
    Serial.println();
  }
}
