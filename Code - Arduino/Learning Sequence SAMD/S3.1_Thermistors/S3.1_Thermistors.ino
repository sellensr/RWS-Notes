// Thermistor Example #3 from the Adafruit Learning System guide on Thermistors 
// https://learn.adafruit.com/thermistor/overview by Limor Fried, Adafruit Industries
// MIT License - please keep attribution and consider buying parts from Adafruit

// Adapted to match Itsy Bitsy M0 capabilities by Rick Sellens
// Assumes reference resistor on high side, supplied from pin 2
// Resolution set to 16 bit

// just calling getThermistor() with no arguments will assume all these settings

// copying and pasting this code to an assignment will generate problems for you and poor marks

// this function uses optional, preinitialized arguments, that require a declaration before the function is used
// optional arguments are a C++ addition and beyond the scope of MECH 217, so your function should have simple arguments.
double getThermistor(unsigned THERMISTORPIN = A5, 
                      double BCOEFFICIENT = 3950, 
                      double THERMISTORNOMINAL = 10000, 
                      double SERIESRESISTOR = 10000,  
                      double TEMPERATURENOMINAL = 25){
  float analog = analogRead(THERMISTORPIN);
 
  // convert the value to resistance
  float frac = 65536 / analog - 1;
  float res = SERIESRESISTOR / frac;

  float steinhart;
  steinhart = res / THERMISTORNOMINAL;          // (R/Ro)
  steinhart = log(steinhart);                   // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                    // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                  // Invert
  steinhart -= 273.15;                          // convert to C
  
  return steinhart;
}

void setup(void) {
  Serial.begin(115200);
  while(!Serial && millis() < 10000);
  pinMode(2,OUTPUT);          // set pin 2 high for the reference resistor
  digitalWrite(2,HIGH);
  analogReadResolution(16);   // use as high a resolution as is supported
}
 
void loop(void) {
  Serial.println(getThermistor(A5, 3950));
  delay(1000);
}
