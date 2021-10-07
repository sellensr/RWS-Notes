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
