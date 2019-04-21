/*
modified from fft_adc_serial.pde
you need to make sure FFT is installed in your libraries folder
guest openmusiclabs.com 7.7.14
example sketch for testing the fft library.
it takes in data on ADC0 (Analog0) and processes them
with the fft. the data is sent out over the serial
port at 115.2kb.
*/

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to FFT_N point fft, number of points must be power of 2

#include <FFT.h> // include the library

void setup() {
  Serial.begin(115200); // use the serial port
}

void loop() {
  static int count = 0;
  static long last = 0;
  double dt = 0.001;
  double f = 100.;
    for (int i=0;i<FFT_N;i++){
      // create the data for the openmusic FFT
      fft_input[2*i] = sin(2.*3.141592654 * f * i * dt) * 1000;   // put real data into even bins
      fft_input[2*i+1] = 0;  // set odd bins to imaginary data
    }

    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft
  count++;
  if(count > 100){
    count = 0;
    Serial.println("start");
    for (byte i = 0 ; i < FFT_N/2 ; i++) { 
      Serial.print(i / (FFT_N * dt)); Serial.print(" ");
      Serial.println(fft_log_out[i]); // send out the data
    }
    Serial.print(FFT_N); Serial.print(" points took "); Serial.print(millis()-last); Serial.println("ms for 100 FFTs, now Again\n\n");
    last = millis();
  }
    
}
