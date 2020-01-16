#define PI 3.141592654

// declare the existence of external global variables defined in MECH217C.c
extern unsigned long timeNow, timeLast;
extern float timeNowS;


void startLoop(){                     // call whenever you start the loop
  timeLast = timeNow;                 // save the time of the last loop start
  timeNow = micros();                 // this is the time that the current loop started
  timeNowS = timeNow/1000000.;        // timeNow, but in Seconds
}

float AtoV(unsigned a,float vref){    // Convert analogRead() value to volts
  a = a%1024;                         // Force into range, assumes 10 bit
  return (a + 0.5) / (1024) * vref;
}

float VtoTMP36(float v){              // Convert voltage to temperature for TMP36
  return (v - 0.5) * 100;             // 0.5V for 0C, increasing by 10 mV/C (datasheet)
}                                     // or 0.010 V/C, or inverting, 100 C/V

/*  Functions for generating time series from discrete steps, pulses, etc
 * 
 */
float stepTS(float t, float tStep){
  if(t < tStep) return 0.0;
  else return 1.0;
}

float sqPulseTS(float t, float tStart, float tStop){
  if(tStop < tStart) return sqPulseTS(t,tStop,tStart);
  else return stepTS(t,tStart)-stepTS(t,tStop);
}

float sinPulseTS(float t, float tStart, float tStop){
  if(tStop < tStart) return sinPulseTS(t,tStop,tStart);
  else if(t < tStart || t > tStop) return 0.0;
  else{
    float period = tStop - tStart + 1;
    float tRel = t - tStart;
    return sin(3.1416 * tRel/period);
  }
}

float normpdf(float x){
  return exp(-x*x/2) / sqrt(2*PI); 
}

float normPulseTS(float t, float tMean, float tStd){
    return normpdf((t - tMean) / tStd) / normpdf(0);
}


float randn(){
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
