
void setup() {
  Serial.begin(115200);
}

void loop(){
  f();
  g();
  Serial.print("\n");
  delay(5000);
}

void f() {       // a function called f() that takes no arguments and returns nothing (void)

  // Local variables are allocated memory space every time the function is called.
  // It may be the same space, or it may be different, so no guarantees and weird
  // effects can crop up with uninitialized variables. DON'T DO IT!
  int test;     // uninitialized local variables - you can use these only inside the function
  int t[5];     // and you can't be sure what the value is until you initialize them.

  // These variables will be initialized fresh every time the function is called.
  int itest = 5;               // initialized local variables - you can use these only inside the function
  int it[ ] = {1, 2, 3, 4, 5}; // and they will have these values unless you change them.
  int itt[5];                  // Declare and immediately initialize in a for loop.
  for(int i = 0;i < 5;i++) itt[i] = i*i;

  Serial.print("in f() test = ");
  Serial.print(test);  Serial.print(", t = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(t[i]); Serial.print(", ");
    t[i] += i;
  }
  Serial.print("\n");

  Serial.print("in f() itest = ");
  Serial.print(itest);  Serial.print(", it = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(it[i]); Serial.print(", ");
    it[i] += i;
  }
  Serial.print("\n");

  Serial.print("in f() itest = ");
  Serial.print(itest);  Serial.print(", itt = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(itt[i]); Serial.print(", ");
    itt[i] += i;
  }
  Serial.print("\n");
}

void g() {      // a function called g() that takes no arguments and returns nothing (void)
  // These variables have the same names, but they aren't shared with f()
  int itest = 25;               
  int it[ ] = {21, 22, 23, 24, 25}; 
  
  Serial.print("in g() itest = ");
  Serial.print(itest);  Serial.print(", it = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(it[i]); Serial.print(", ");
    it[i] += i;
  }
  Serial.print("\n");
}


