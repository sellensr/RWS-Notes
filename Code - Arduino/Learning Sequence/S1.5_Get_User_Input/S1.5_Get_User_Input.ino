// All this Serial.print() will get tedious -- define some shortcuts
#define P Serial.print
#define PCS Serial.print(", ");Serial.print
#define PCSL Serial.print(", ");Serial.println

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  // set the serial port speed
}

void loop() {
  Serial.setTimeout(5000);        // Wait 5 seconds before moving on
  P("\nEnter a String\n");          // Read all the characters you can until <Return>
  String scratch = Serial.readStringUntil('\n');    // or timeout...
  P("scratch = ");
  P(scratch); 
  P("\n");
  
  P("\nEnter an Integer\n");
  int i = Serial.parseInt();      // Keep trying to get an int until timeout
  P("i = ");                      // print the results
  P(i); 
  P("\n");
  
  P("\nEnter a Float\n");           
  float x = Serial.parseFloat();  // Keep trying to get a float until timeout
  P("x = ");                      // print the results
  P(x); 
  P("\n");

  P("\nEnter some characters\n");
  while(!Serial.available());     // Wait until there are characters
  while(Serial.available()){      // read them until they are gone
    char c = Serial.read();       // read a character
    P("c = ");                    // print the results
    P(c); 
    P("\n");
  }

}
