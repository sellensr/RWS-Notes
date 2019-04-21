#define MAXLINE 100
int max;

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
  int len = 0;
  char line[MAXLINE];            /* current input line */
  static char longest[MAXLINE];  /* longest line saved here */

  Serial.print("Memory Addresses:\n             external variable     max: ");
  Serial.print((long) &max);
  Serial.print(                 "\nsetup()        static variable longest: ");
  Serial.print((long) &longest[0]);
  Serial.print(                 "\nsetup()     automatic variable    line: ");
  Serial.print((long) &line[0]);
  Serial.print(                 "\nsetup()     automatic variable     len: ");
  Serial.print((long) &len);
  Serial.println("\n Using recursion (functions calling themselves) will \n eventually result in a stack collision with unpredictable results");
}

void loop() {               // Arduino has a built in continuous loop()
  int t = micros() / 200000 - 5;
  int s = sumDigits(t);
  Serial.print("\nSum of digits less than or equal to ");
  Serial.print(t);
  Serial.print(" is ");
  Serial.println(s);
  delay(2000);
}

int sumDigits(long d){  // uses recursion -- eventually there will be a stack collision!
  long dm = 0;
  Serial.print(                 "\nsumDigits() automatic variable      dm: ");
  Serial.print((long) &dm);
  if(d > 0){
    dm = sumDigits(d-1);
    return d + dm;
  }
  else return 0;
}
