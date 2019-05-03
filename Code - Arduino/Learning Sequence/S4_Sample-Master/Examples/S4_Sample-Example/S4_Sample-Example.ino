#include <S4_Sample.h>  // look down the library path

S4_Sample samp = S4_Sample();

void setup() {
  samp.begin(115200);
  P("\n\nS4_Sample Library Test Sketch\n\n");
  P(samp.bytesFree());
  P(" bytes of memory free (approximately)\n");
}

void loop() {
  samp.helloWorld();
  delay(2345);
}
