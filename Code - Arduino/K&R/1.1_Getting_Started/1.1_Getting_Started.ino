// An Arduino Version of K&R Example 1.1
// Adapted from The C Programming Language, 2nd Ed., 
// Brian W. Kernighan, Dennis M. Ritchie, Prentice Hall

//
// lines beginning with // are comments to help make the code readable
// #include <stdio.h>       // is not required as the Arduino IDE automatically
                            // includes definitions for core functions 
#include <Wire.h>           // include the I2C communications library as an
                            // example we will use later

//main(){                   // is already included by the IDE
//  setup();                // call the setup() function once
//  while(true) loop();     // call the loop function over and over again forever
//}                         // if you try to compile code without a loop() or setup()
                            // function you will get an error like this.
                            // In function `main': ...: undefined reference to `loop'

// All Arduino sketches must include setup() and loop(), the same way all c programs must include main()
 
void setup()                // define a function named setup() 
                            // that receives no argument [empty ()]
                            // and returns no result [void]

{                           // statements of setup are enclosed in { }
                            
  Serial.begin(115200);     // setup() calls the Serial library function begin()
                            // to initialize the Serial library for communication
                            // set the speed to 115200 baud or bits per second
                            // be sure to set the Serial Monitor to the same speed
                            
  Serial.print("hello, world\n");
                            // setup() calls the Serial library function print()
                            // to print this sequence of characters
                            // \n represents the newline character
}

void loop() {
  // put your main code here, to run repeatedly:

}

void rick(void){}
