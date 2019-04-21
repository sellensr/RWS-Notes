// Global Variables have global scope, are visible and usable throughout the sketch.
// They will keep their values and not be reinitialized.
int itest = 5;               // initialized global variables - you can use these anywhere
int it[ ] = {1, 2, 3, 4, 5}; // and they will have these values unless you change them.


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  locals();
  globals();
  Serial.print("\n");
  delay(5000);
}

void locals() {
  // Local Variables declared in the function only have local scope. Even if they have names
  // the same as the variables in another function, or globally, they will still be 
  // independent variables inside the function. Local names the same as global names 
  // can be confusing -- probably best avoided unless there is a good reason.
  static int test = 5;               // static variables keep their values between
  static int t[ ] = {1, 2, 3, 4, 5}; // function calls -- they remember
  int itest = 5;                     // regular variables are reinitialized every
  int it[ ] = {1, 2, 3, 4, 5};       // time the function is called

  Serial.print("in locals() test = ");
  Serial.print(test);  Serial.print(", t = ");
  // i will only have scope inside the for loop. This will protect you from accidentally 
  // changing another variable i that may have been declared in the function or globally 
  for (int i = 0; i < 5; i++) {
    Serial.print(t[i]); Serial.print(", ");
    t[i] += i;
  }
  test += 2;
  Serial.print(" retains changes because these are declared static.\n");

  Serial.print("in locals() itest = ");
  Serial.print(itest);  Serial.print(", it = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(it[i]); Serial.print(", ");
    it[i] += 1;
  }
  itest += 4;
  Serial.print(" are reinitialized every time the function is called.\n");

}

void globals() {
  Serial.print("in globals() itest = ");
  Serial.print(itest); Serial.print(", it = ");
  for (int i = 0; i < 5; i++) {
    Serial.print(it[i]);
    Serial.print(", ");
    it[i] += 3 * i;
  }
  itest += 6;
  Serial.print(" retains different changes because these are the global variables.\n");
}


