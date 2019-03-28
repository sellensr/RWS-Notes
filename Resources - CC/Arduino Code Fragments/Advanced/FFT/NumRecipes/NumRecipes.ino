// This code demonstrates and times the FFT code originally from Numerical Recipes in C
// The main data string is stored as floats rather than doubles to make it smaller
#define NFFT  128
#define NPTS  260

void setup(){
  Serial.begin(115200);
  Serial.print("\n\nNumRecipes FFT\n\n");
}

void loop(){
  static int count = 0;
  static long last = 0;
  float dat[NPTS];
  double dt = 0.002;
  double f = 200.;
  for (int i=0;i<NFFT;i++){
    int real = i*2+1;
    int imag = i*2+2;
    dat[real] = sin(2.*3.141592654 * f * i * dt);
    dat[imag] = 0.;
  }
  four1(dat,NFFT,1);
  psd(dat,NFFT);
  count++;
  if(count > 100){
    count = 0;
    for (int i=0;i <= NFFT/2;i++){
      Serial.print(i/(NFFT*dt)); Serial.print(" "); Serial.println(dat[i]);
    }
    Serial.print(NFFT); Serial.print(" points took "); Serial.print(millis()-last); Serial.println("ms for 100 FFTs and PSD, now Again\n\n");
    delay(1000);
    last = millis();
  }
}

/************************************************
* FFT code from the book Numerical Recipes in C *
* Visit www.nr.com for the licence.             *
************************************************/
// changed out the clik_for() to for() and converted to float data for more compact storage

// The following line must be defined before including math.h to correctly define M_PI
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI	M_PI	/* pi to machine precision, defined in math.h */
#define TWOPI	(2.0*PI)

/*
 FFT/IFFT routine. (see pages 507-508 of Numerical Recipes in C)

 Inputs:
    data[]:   array of complex* data points of size 2*NFFT+1.
              data[0] is unused,
              the n'th complex number x(n), for 0 <= n <= length(x)-1, is stored as:
                data[2*n+1] = real(x(n))
                data[2*n+2] = imag(x(n))
              if length(Nx) < NFFT, the remainder of the array must be padded with zeros
       nn:    FFT order NFFT. This MUST be a power of 2 and >= length(x).
    isign:    if set to 1, computes the forward FFT
              if set to -1, computes Inverse FFT - in this case the output values have
                to be manually normalized by multiplying with 1/NFFT.
 Outputs:
   data[]:    The FFT or IFFT results are stored in data, overwriting the input.
*/

void four1(float data[], int nn, int isign)
{
    int n, mmax, m, j, istep, i;
    float wtemp, wr, wpr, wpi, wi, theta, tempr, tempi;

    n = nn << 1;   j = 1;
    for (int i = 1; i < n; i += 2) {
	if (j > i) {
	    tempr = data[j];     data[j] = data[i];      data[i] = tempr;
	    tempr = data[j+1];   data[j+1] = data[i+1];  data[i+1] = tempr;
	}
	m = n >> 1;
	while (m >= 2 && j > m) {   j -= m;  m >>= 1;  }
	j += m;
    }
    mmax = 2;
    while (n > mmax) {
        // considerable opportunity for speedup here by having sin() precalculated for a bunch integer divisions of TWOPI
	istep = 2*mmax;           theta = TWOPI/(isign*mmax);   wtemp = sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;   wpi = sin(theta);             wr = 1.0;     wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
		j =i + mmax;
		tempr = wr*data[j] - wi*data[j+1];  tempi = wr*data[j+1] + wi*data[j];  data[j] = data[i]   - tempr;
		data[j+1] = data[i+1] - tempi;      data[i] += tempr;                   data[i+1] += tempi;
	    }
	    wr = (wtemp = wr)*wpr - wi*wpi + wr;    wi = wi*wpr + wtemp*wpi + wi;
	}
	mmax = istep;
    }
}

void psd1(float data[], int nn){
  // calculate the power spectrum in place from FFT output data
  // continues to ignore data[0] in the long tradition of FORTRAN
  // Does not scale the results to reflect the unknown frequency range or the now unknown original total power.
  // data[1] = data[1] * data[1] + data[2] * data[2], the sum of the squares for f = 0
  // data[2] = data[4]^2  + data[3]^2    + data[2N]^2       + data[2N-1]^2     the sum of the squares for f = 1/(N dt)
  // data[i] = data[2i]^2 + data[2i-1]^2 + data[2(N-i)+2]^2 + data[2(N-i)+1]^2 the sum of squares for f = (i-1)/(N dt)
  //      where i goes up to N/2 and then
  // data[N/2+1] = data[N+1]^2 + data[N+2]^2 the sum of the squares for f = 1/(2 dt)
  //      and set all the rest to zero to avoid confusion
  data[1] = data[1]*data[1] + data[2]*data[2];
  for(int i=2;i <= nn/2;i++) data[i] = data[2*i]*data[2*i] + data[2*i-1]*data[2*i-1]; 
  data[nn/2+1] = data[nn+1]*data[nn+1] + data[nn+2]*data[nn+2];
  for(int i = nn/2+2;i <= nn*2;i++) data[i]=0;
 }

void psd(float data[], int nn){
  // calculate the power spectrum in place from FFT output data
  // uses data[0], ignoring the long tradition of FORTRAN, but providing a much more elegant result
  // Does not scale the results to reflect the unknown frequency range or the now unknown original total power.
  // data[0] = data[1] * data[1] + data[2] * data[2], the sum of the squares for f = 0
  // data[1] = data[4]^2  + data[3]^2    + data[2N]^2       + data[2N-1]^2     the sum of the squares for f = 1/(N dt)
  // data[i] = data[2i+2]^2 + data[2i+1]^2 + data[2(N-i)]^2 + data[2(N-i)-1]^2 the sum of squares for f = i/(N dt)
  //      where i goes up to (N/2-1) and then
  // data[N/2] = data[N+1]^2 + data[N+2]^2 the sum of the squares for f = 1/(2 dt)
  //      and set all the rest to zero to avoid confusion
  data[0] = data[1]*data[1] + data[2]*data[2];
  for(int i=1;i < nn/2;i++) data[i] = data[2*i+2]*data[2*i+2] + data[2*i+1]*data[2*i+1]; 
  data[nn/2] = data[nn+1]*data[nn+1] + data[nn+2]*data[nn+2];
  for(int i = nn/2+1;i <= nn*2;i++) data[i]=0;
 }

