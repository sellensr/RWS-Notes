// An Arduino Version of K&R Example 1.5.4
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

#define IN   1   /* inside a word */
#define OUT  0   /* outside a word */
 /* count lines, words, and characters in input */

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
}

void loop() {               // Arduino has a built in continuous loop()
  static int nc = 0;        // static variables hold their values between calls
  static int nl = 0;        // 
  static int nw = 0;        // number of words in the line
  static int state = OUT;   // not in a word yet
  int c;
  
  c = Serial.read();        // Serial.read replaces getchar(). Returns -1 if nothing there
  if(c != -1){              // if there is a character available 
    Serial.write(c);        // Serial.write replaces putchar()
    ++nc;                   // increment the character count
    if(c == '\n')
      ++nl;
    if(c == ' ' || c== '\n' || c == '\t') // a space or a newline or a tab ends a word
      state = OUT;
    else if(state == OUT){  // other characters mean the start of a new word
      state = IN;
      ++nw;
    }
    delay(5);               // go slowly enough to let the sender finish
  }
  else if (nc != 0){        // if there were characters counted
    Serial.print(nc);       // print the character count if not zero
    Serial.print(", ");
    Serial.print(nw);       // print the character count if not zero
    Serial.print(", ");
    Serial.println(nl);     // print the number of lines so far
    nc = nw = 0;            // reset the character and word counts
  }
}
