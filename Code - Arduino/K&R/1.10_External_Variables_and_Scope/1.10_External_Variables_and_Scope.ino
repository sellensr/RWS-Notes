// An Arduino Version of K&R Example 1.10
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

#define MAXLINE 100  // smaller than K&R because of limited memory
int getline(void); // *declares* the function without defining it
void copy(void);   // *declares* the function without defining it
      // not required in Arduino if the function is in the same sketch file.


void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
}

int max = 0;            // external variables are visible everywhere!
char line[MAXLINE];     /* current input line */
char longest[MAXLINE];  /* longest line saved here */

void loop() {               // Arduino has a built in continuous loop()
  int len = 0;

  len = getline();
  if (len > max){
    max = len;
    copy();
  }
  
  if( len > 0 ){
    Serial.println(max);
    Serial.print(longest);
    Serial.println(line);
  }
}

/* getline:  specialized version */
int getline()      // *defines* the function
{
  int c, i;
  for (i=0; i<MAXLINE-1 && (c=Serial.read())!=-1 && c!='\n'; ++i){
    line[i] = c;
    delay(5);
  }
  if (c == '\n') {
    line[i] = c;
    ++i;
  }
  line[i] = '\0';
  return i;
}

/* copy:  specialized version */
void copy()  // *defines* the function
{
  int i;
  i = 0;
  while ((longest[i] = line[i]) != '\0')
    ++i;
}
