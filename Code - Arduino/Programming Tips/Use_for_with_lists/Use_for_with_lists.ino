#include "RTClib.h"
#define N_AVG 3
#define N_PORTS 3

int portList[] = {A4, A3, A1, 999};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial && millis() < 5000);
  Serial.print("Use for loops with lists\n\n");

  double sum[10] = {0.0};
  int j = 0;
  for(j = 0; portList[j] < 100; j++){  
    for(int i = 0; i < N_AVG; i++){
      int a = analogRead(portList[j]);
      Serial.printf("i = %d, j = %d, AX --> %d\n",i,j,a);
      sum[j] += a;
      delay(1000);
    }
    sum[j] /= N_AVG;
    Serial.printf("mean = %8.2f\n",sum[j]);
  }
  Serial.printf("We checked %d analog ports\n",j);
}

void loop() {
  // put your main code here, to run repeatedly:

}
