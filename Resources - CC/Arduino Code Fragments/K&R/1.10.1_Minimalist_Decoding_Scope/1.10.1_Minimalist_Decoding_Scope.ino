#define MAXLINE 100
int max;

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
  int len = 0;
  char line[MAXLINE];            /* current input line */
  static char longest[MAXLINE];  /* longest line saved here */

  Serial.print("Memory Addresses:\n external variable     max: ");
  Serial.print((long) &max);
  Serial.print(                 "\n   static variable longest: ");
  Serial.print((long) &longest[0]);
  Serial.print(                 "\nautomatic variable    line: ");
  Serial.print((long) &line[0]);
  Serial.print(                 "\nautomatic variable     len: ");
  Serial.print((long) &len);
}

void loop() {               // Arduino has a built in continuous loop()
}
