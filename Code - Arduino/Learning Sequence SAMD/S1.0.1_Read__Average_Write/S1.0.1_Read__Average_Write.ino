#define NAVG 10000
#include <RTClib.h>     // oddly, enables %f in Serial.printf()

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);   // set the serial port speed
  while(!Serial && millis() < 5000);         // wait for the USB to wake up
  Serial.print("\n\nS1.0.1 Read / Average / Write for Itsy Bitsy M0\n\n");
  // try different analog reference voltages by uncommenting only one of these lines. 
  // https://www.arduino.cc/reference/tr/language/functions/analog-io/analogreference/
  // Note that unlike the UNO, you can't read the internal reference voltages on the Aref pin.
  analogReference(AR_DEFAULT);
//  analogReference(AR_INTERNAL1V0);
//  analogReference(AR_INTERNAL1V65);
//  analogReference(AR_INTERNAL2V23);
//  analogReference(AR_EXTERNAL);       // Avoid external references unless really needed
  // try different analog bit resolutions, note the default is 10 for UNO compatibility
  analogReadResolution(12);     // 0-4095, native resolution for the M0
//  analogReadResolution(16);     // 0-65536, but will only reach 65520 on the M0

  // Use pins A0 and A2 for power supply and ground so you can plug a potentiometer
  // directly into pins A0, A1, A2 as a stable calibration voltage source.
  pinMode(A0,OUTPUT);
  digitalWrite(A0,HIGH);
  pinMode(A2,OUTPUT);
  digitalWrite(A2,LOW);
}

void loop() {
  double a = 0;
  for(int i = 0;i < NAVG;i++) a += analogRead(A1);
  a /= NAVG;
  unsigned long tus = micros();
  double t = tus/1000000.;
  Serial.printf("%11u us or %11.6f s since start, %8.2f average analogRead()\n",tus,t,a);
  delay(1000);
}
