// An Arduino Version of K&R Example 1.9, extended to map memory addresses
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

#define MAXLINE 100  // smaller than K&R because of limited memory
int getline(char line[], int maxline); // *declares* the function without defining it
void copy(char to[], char from[]);    // *declares* the function without defining it
              // not required in Arduino if the function is in the same sketch file.


void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
}

int max = 0;

void loop() {               // Arduino has a built in continuous loop()
  int len = 0;
  char line[MAXLINE];            /* current input line */
  static char longest[MAXLINE];  /* longest line saved here */

  len = getline(line,MAXLINE);
  if (len > max){
    max = len;
    copy(longest,line);
  }
  
  if( len > 0 ){
    Serial.println(max);
    Serial.print(longest);
    Serial.println(line);
    Serial.print("Memory Addresses:\n        external variable     max: ");
    Serial.print((long) &max);
    Serial.print(                 "\nloop()    static variable longest: ");
    Serial.print((long) &longest[0]);
    Serial.print(                 "\nloop() automatic variable    line: ");
    Serial.print((long) &line[0]);
    Serial.print(                 "\nloop() automatic variable     len: ");
    Serial.print((long) &len);
  }
}

/* getline:  read a line into s, return length */
int getline(char s[], int lim)      // *defines* the function
{
  int c, i;
  for (i=0; i<lim-1 && (c=Serial.read())!=-1 && c!='\n'; ++i){
    s[i] = c;
    delay(5);
  }
  if (c == '\n') {
    s[i] = c;
    ++i;
  }
  s[i] = '\0';
  return i;
}

/* copy:  copy ′from′ into ′to′; assume to is big enough */
void copy(char to[], char from[])  // *defines* the function
{
  int i,j,k;
  Serial.print(                 "\ncopy() automatic variable       i: ");
  Serial.println((long) &i);
  i = 0;
  while ((to[i] = from[i]) != '\0')
    ++i;
}
