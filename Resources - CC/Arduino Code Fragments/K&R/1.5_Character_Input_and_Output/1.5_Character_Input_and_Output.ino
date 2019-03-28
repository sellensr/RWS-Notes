// An Arduino Version of K&R Example 1.5
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
}

long nc = 0;                // an external variable keeps count between loop() calls

void loop() {               // Arduino has a built in continuous loop() function
  int c;
  c = Serial.read();        // Serial.read replaces getchar(). Returns -1 if nothing there
  if(c != -1){              // if there is a character available 
    Serial.write(c);        // Serial.write replaces putchar()
    ++nc;                   // increment the character count
    delay(5);               // go slowly enough to let the sender finish
  }
  else if (nc != 0){        // if there were characters counted
    Serial.println(nc);     // print the character count if not zero
    nc = 0;
  }
}
