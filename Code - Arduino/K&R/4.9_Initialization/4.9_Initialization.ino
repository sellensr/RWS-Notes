// An Arduino Version related to K&R Section 4.9 on Initialization
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall
/*
 * "In the absence of explicit initialization, external and static 
 * variables are guaranteed to be initialized to zero; automatic 
 * and register variables have undefined (i.e., garbage) initial values."
 * 
 * "If there are fewer initializers for an array than the number 
 * specified, the missing elements will be zero for external, 
 * static, and automatic variables."
 */

int a[] = {1,2,3};        // automatically a three element array
int b[10];                // a 10 element array automatically initialized to zero
int c[10] = {1,4,9};      // a 10 element array with 1, 4, 9 followed by zeros

void setup() {
  Serial.begin(115200);     // setup() calls the Serial library function begin()
  printArray("a",a,10);
  printArray("b",b,10);
  printArray("c",c,10);
}

void loop() {               // Arduino has a built in continuous loop()
  int la[] = {11,12,13};    // a three element array 
  int lb[10];               // a 10 element array that could contain garbage
  int lc[10] = {11,14,19};  // a 10 element array with 11, 14, 19, followed by zeros
  printArray("la",la,10);
  printArray("lb",lb,10);
  printArray("lc",lc,10);
  delay(10000);
}

void printArray(char *lab, int *a,int n){
  Serial.print(lab); Serial.print(": ");
  for(int i = 0;i < n;i++){
  Serial.print(a[i]); Serial.print(", ");
  }
  Serial.print("\n");
}
  
