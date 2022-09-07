#include "RTClib.h"

// IDE 2.0 rc9.2 requires Name:value formatting of plotted variables and \r\n line terminations

int random_variable;
int static_variable = 500;
	
void setup() {
  Serial.begin(115200);
}

void loop() {
  random_variable = random(0, 1000);
	  // Serial.print("Variable 1:");
	  // Serial.print(random_variable);
	  // Serial.print(",");
	  // Serial.print("Variable 2:");
	  // Serial.println(static_variable);
    Serial.printf("A:%d,Var2:%d\r\n",random_variable,static_variable);
    delay (100);
	}