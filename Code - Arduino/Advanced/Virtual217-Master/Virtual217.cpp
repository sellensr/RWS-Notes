#include "Virtual217.h"

#define PSEUDO_GROUND   1.239
#define GAIN          100.0
#define USB_VOLTAGE     5.043
#define V33             3.296
#define V11             1.086

#ifndef INTERNAL      // dummy code to let it compile on M0
#define INTERNAL 123
#endif

float VrefX = USB_VOLTAGE;

float randnV(){
  // A pseudo random number with a pseudo Gaussian distribution, mean = 0, sd = 1
  // You can test the idea with this python code:
  //  # a pretty good fake Gaussian from random integers
  //  n = 10000000
  //  tot = random.randint(0,1001,n)
  //  for i in range(0,15): tot += random.randint(0,1001,n)
  //  tot -= 8000
  //  tot = tot / 1156
  //  print(mean(tot),std(tot))
  //  hist(tot,bins=50)
  unsigned long tot = 0;
  for(int i = 0;i < 16;i++) tot += random(1001);
  return (tot - 8000.)/1156.;
}

// All of these voltage functions are internal to the module and not accessible outside
float V(float ts){
  float V = 0;

  return V;
}

float Vpseudo(float ts){  // the voltage of the amplifier pseudo-ground
  float V = PSEUDO_GROUND;
  V += randnV()*0.005;
  return V;
}

float VTMP36(float ts){   // a TMP36 on a coffee cup, water poured at 10 seconds
  float Ta = 22.;
  float Tc = 80.;
  float Ts = Ta;
  float V = 0;
  unsigned tsu = ts;
  float tw = ts - (tsu/120)*120 - 10;              // repeat every two minutes
  if(tw < 0) Ts = Ta;                             // initially at ambient
  else if(tw < 60) Ts = Tc - exp(-tw/15)*(Tc-Ta); // decay towards cup T
  else Ts = Ta - exp(-(tw-60)/20)*(Ta-Tc);        // decay back to ambient
  V = Ts * 0.010 + 0.5 + randnV()*0.005;           // voltage with 10mV noise
  return V;
}

float Vtc(float ts){
  float Ta = 22.;
  float Tc = 80.;
  float Ts = Ta;
  float V = 0;
  unsigned tsu = ts;
  float tw = ts - (tsu/120)*120 - 10;              // repeat every two minutes
  if(tw < 0) Ts = Ta;                             // initially at ambient
  else if(tw < 60) Ts = Tc - exp(-tw/4)*(Tc-Ta); // decay towards cup T
  else Ts = Ta - exp(-(tw-60)/8)*(Ta-Tc);        // decay back to ambient
  V = Vpseudo(ts) + (Ts-Ta)/(Tc-Ta) * 1.5 + randnV()*0.005;   // voltage with 10mV noise
  return V;
}

float Vpot(float ts){    // a potentiometer being turned manually through 180 degrees
  float V = 0;
  float tw = ts - (((int) ts)/20)*20;                   // repeat every 20 seconds
  if(tw < 3.1416) V = 0.8;                           
  else if(tw < 2 * 3.1416) V = 0.8 + 1.50 * (cos(tw) + 1);
  else if(tw < 3 * 3.1416) V = 3.8;
  else if(tw < 5 * 3.1416) V = 2.3 - 1.5 * sin(0.5*tw);
  else V = 0.8;
  V += randnV()*0.005;
  return V;
}

float Vload(float ts){     // a load cell subject to increasing loads, then step loads
  unsigned tsu = ts;
  float V = 0, m = 0;
  float tw = ts - (tsu/60)*60;                         // repeat every minute
  if(tw < 10) m = (tsu%60)/2;                    // 0, 1, 2, 3, 4 kg
  else if(tw < 12) m = 1.5;                      // back to 1.5, then jump to 2.5, 4.5, 0
  // frequency prop to m**-0.5, tau prop to m
  else if(tw < 25) m = 2.5 - exp(-(tw-12)/2.5)*cos((tw-12)*3.1416 *10/sqrt(2.5));
  else if(tw < 45) m = 4.5 - 2 * exp(-(tw-25)/4.5)*cos((tw-25)*3.1416 *10/sqrt(4.5));
  else m = 0 - 4.5 * exp(-(tw-45)/.1)*cos((tw-45)*3.1416 *10/sqrt(.1));
  m += randnV()*0.02;
  V = 1.24 + m * 0.287 + randnV() * 0.005;
  return V;
}

float Vacc(float ts){   // an accelerometer being held flat, then on each edge, then moving
  unsigned tsu = ts;
  float V = 0, m=0;
  float tw = ts - (tsu/60)*60;                         // repeat every minute
  if(tw < 5) m = 0;
  else if(tw < 10) m = 1.0;
  else if(tw < 15) m = -1.0;
  else if(tw < 25) m = 0;
  else if(tw < 35) m = 0.8 * sqrt(max(0,sin((tw-25)/10 * 3.1416)))*sin((tw-25)*3.1416);
  else if(tw < 40) m = 0;
  else if(tw < 45) m = 2.8 * sqrt(sqrt(max(0,sin((tw-40)/5 * 3.1416))))*sin((tw-40)*5*3.1416);
  else m = 0;
  m += randnV()*0.004;
  V = 1.57 + m * 0.487 + randnV() * 0.005;
  return V;
}

float Vpress(float ts){
  float V = 0;

  return V;
}

void analogReferenceV(unsigned src){                 // a substitute for analogReference()
  if(src == INTERNAL) VrefX = V11;
  else if(src == EXTERNAL) VrefX = V33;
  else VrefX = USB_VOLTAGE;
}

unsigned long analogReadV(unsigned port){        // a substitute for analogRead()
  // Returns data as if there were various circuits attached to the Arduino
  // with predefined events corresponding to some of the MECH 217 lab activities.
  unsigned long t = micros();
  float ts = t /1000000.; // time in seconds
  int A = 0;              // so it can go a little negative and round back up to 0
  float V = 0;            // voltage on the pin in volts
  switch(port){
    case A0:
      V = VTMP36(ts);     // a TMP36 on a coffee cup, water poured at 10 seconds
      break;
    case A1:
      V = Vpot(ts);       // a potentiometer being turned manually through 180 degrees
      break;
    case A2: 
      V = Vacc(ts);       // an accelerometer being held flat, then on each edge, then moving
      break;
    case A3: 
      V = Vload(ts);      // a load cell subject to increasing loads, then step loads
      break;
    case A4:
      V = Vpseudo(ts);    // the voltage of the amplifier pseudo-ground
      break;
    case A5:
      //V = sin(ts)*0.50 + 4.5; // a  small sine wave in case you need one
      V = Vtc(ts);        // the voltage of a thermocouple on a coffee cup 
      break;
    default:
      V = 0;
      break;
  }
  A = 1023 * V/VrefX + randnV()*0.75;               // conversion bit variation
  A = min(1023,A);                                  // force into range
  A = max(0,A);
  return A;
}
