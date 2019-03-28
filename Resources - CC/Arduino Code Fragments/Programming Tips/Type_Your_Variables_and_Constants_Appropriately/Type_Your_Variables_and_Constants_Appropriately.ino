void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // floats and doubles are the same for the UNO
  // floats are 32 bit representations of decimal numbers
  static float x = 27;
  // ints are positive and negative integers
  Serial.print(x); Serial.print(", ");
  Serial.print("\n");
}


