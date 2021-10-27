// This code demonstrates and times the FFT code originally from Numerical Recipes in C
// and compares it to the integer FFT provided by ARM through Adafruit for the M0.
// The main data string is stored as floats rather than doubles to make it smaller
#define NPTS  2060      // the number of points in the array needs to be at least 2*NFFT + 4
float dat[NPTS];        // data space as a float to save storage v double
short int  idat[NPTS];  // data space as an int array for Adafruit_ZeroFFT, 16 bit ints
double dt = 0.0001;     // time between samples in seconds
int NFFT = 512;         // the number of points in time for the FFT data

#include "Adafruit_ZeroFFT.h"

void setup(){
  Serial.begin(115200);
  while(!Serial && millis() < 10000);
  Serial.print("\n\nNumRecipes_v_ARM_Integer_FFT_Test\n\n");
}

void loop(){
  double f = 200.;        // frequency of sine wave in Hz
  double f2 = 60.;        // another frequency of in Hz
  double f3 = 1200.;      // another frequency of in Hz

  // Generate a simulated signal and plug it into the data arrays
  Serial.print("__t_[s]__ signal_[-] _signal_[-]_ "); Serial.print(NFFT); Serial.print("_points\n");
  for (int i=0;i<NFFT;i++){                                                               //    dat[0] = ignored
    int real = i*2+1;     // real components go in elements 1, 3, 5, etc. (FORTRAN throwback)   dat[1] = first real data point
    int imag = i*2+2;     // imaginary components go in elements 2, 4, 6, etc.                  dat[2] = first imaginary data point (probably 0)
    dat[real] = 0.0;      // any constant offset should go to first fourier component           dat[3] = second real data point
    dat[real] = sin(2.*3.141592654 * f * i * dt);         // real part gets base signal         dat[4] = second imaginary data point (probably 0)
    dat[real] += 0.3 * cos(2.*3.141592654 * f2 * i * dt); // real part gets more                ...
    dat[real] += 0.1 * sin(2.*3.141592654 * f3 * i * dt); // real part gets more                dat[127] = 64th real data point
    dat[imag] = 0.;                                       // all imaginary points are 0         dat[128] = 64th imaginary data point  
    
    idat[i] = dat[real] * 1400;   // scaled, no imaginary part to integer version ZeroFFT()

    // print the signal so we can see it on the plotter
    Serial.print(i*dt,6);
    Serial.print(" ");
    Serial.print(dat[real]);
    Serial.print(" ");
    Serial.print(idat[i]/1000.);
    Serial.println();
  }

  delay(5000);

  // Process the data array with a floating point Fourier transform
  unsigned long t1 = micros();  // check the time 
  four1(dat,NFFT,1);            // do the transform
  psd(dat,NFFT);                // calculate the power spectral density (sum of squares of components at f) so after processing by psd():
  t1 = micros()-t1;             // how long did it take?                                        dat[0] = DC power spectral density point (f = 0)
                                //                                                              dat[1] = first oscillatory PSD (f = 1 / (n_FFT * delta t)
  unsigned long t2 = micros();  // check the time                                               dat[2] = second oscillatory PSD (f = 2 / (n_FFT * delta t)
  ZeroFFT(idat,NFFT);           // do the integer base transform                                ...
  t2 = micros()-t2;             // take the difference as elapsed time                          dat[32] = 32nd osciallatory PSD (f = 32 / (n_FFT * delta t)
    
  // Print out some results 
  Serial.print("freq_[kHz] log(PSD) log(idat[]^2)\n");
  for (int i=0;i <= NFFT/2;i++){
    Serial.print(i/(NFFT*dt)/1000.,3);    // the corresponding frequency in kHz 
    Serial.print(" ");
    Serial.print(log(dat[i]));       // the power level
    Serial.print(" ");
    Serial.print(log(idat[i] * (float) idat[i] + 1));
    Serial.println();
  }
  Serial.print("freq_[kHz] log(PSD) log(idat[]^2) ");
  Serial.print(NFFT); Serial.print("_points_took_"); 
  Serial.print(t1 / 1000.); Serial.print("_ms_per_FFT_and_PSD_");
  Serial.print(t2 / 1000.); Serial.print("_ms_per_ZeroFFT");
  Serial.println();
  delay(5000);
  dt *= 2;        // longer timestep
  NFFT /= 2;      // fewer points for same length of time
  if(dt > .001){  // revert to start after a few halvings
    dt = .0001;
    NFFT = 512;
  }
}
