// An Arduino Version related to K&R Section 5.7 on Initialization of Multi-dimensional Arrays
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
 * 
 * Note that there is no error generated when arrays are addressed 
 * with indices that exceed the declared bounds of the array.
 */

int a[2][3] = {{1,2,3},{4,5,6}};   // a 2x3 array
int b[2][3];                       // a 2x3 array automatically initialized to zero
int c[2][3] = {1,4,9};             // a 2x3 array with 1, 4, 9 followed by zeros

void setup() {
  Serial.begin(115200);     
  print2DArray("a",a,4);
  print2DArray("b",b,4);
  print2DArray("c",c,4);
}

void loop() {           
  int la[2][3] = {{11,12,13},{14,15,16}};   // a 2x3 array
  int lb[2][3];                             // a 2x3 array not automatically initialized
  int lc[2][3] = {11,14,19};                // a 2x3 array with 1, 4, 9 followed by zeros
  int ld[2][3] = {0};                       // yields an array of zeros, no matter where declared
  print2DArray("la fully initialized = {{11,12,13},{14,15,16}}",la,4);
  print2DArray("lb uninitialized",lb,4);
  print2DArray("lc partly initialized = {11,14,19}",lc,4);
  print2DArray("ld partly initialized = {0}",ld,4);
  delay(10000);
}

void print2DArray(char *lab, int a[2][3],int n){
  Serial.print(lab); Serial.print(":\n ");
  for(int i = 0;i < n;i++){
    for(int j = 0;j < n;j++){
      Serial.print(a[i][j]); Serial.print(", ");
    }
    Serial.print("\n");
  }
  Serial.print("\n");
}
  
