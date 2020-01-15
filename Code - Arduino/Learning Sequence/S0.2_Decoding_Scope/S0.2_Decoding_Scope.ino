// Variable Scoping, extended to map memory addresses
int i = 42;
int j = 3;
int k = 4;
int depth = 0;

void setup() {
  depth++;
  Serial.begin(115200);
  while(!Serial && millis() < 5000);
  show("In setup(), ",i,j,k,&i,&j,&k);
  depth--;
}


void loop() {
  depth++;
  Serial.print("\n\nThe address of depth is:"); Serial.println((long) &depth);
  int i = 36;
  show("In loop()1, ",i,j,k,&i,&j,&k);
  i = f(j);  
  show("In loop()2, ",i,j,k,&i,&j,&k);
  i = g(j);  
  show("In loop()3, ",i,j,k,&i,&j,&k);
  delay(15000);
  depth--;
}


int f(int j) {
  depth++;
  int i = 0;
  show("In    f()1, ",i,j,k,&i,&j,&k);
  for (int i=0; i <= j; ++i){
    show("In    f()2, ",i,j,k,&i,&j,&k);
    k++;
  }
  if(j == 0) i = 0;
  else i = f(j-1);
  depth--;
  return i;
}

int g(int k) {
  depth++;
  int i = 88;
  show("In    g(), ",i,j,k,&i,&j,&k);
  k = f(1);
  depth--;
  return k;
}


void show(char *s,int ii,int jj,int kk,int *iii,int *jjj,int *kkk){
  for(int i = 0;i<depth;i++) Serial.print("\t");
  Serial.print(s);
  Serial.print("Integers i, j, k: ");
  Serial.print(ii); Serial.print(", ");
  Serial.print(jj); Serial.print(", ");
  Serial.print(kk);
  Serial.print("    Pointers to Integers i, j, k: ");
  Serial.print((long) iii); Serial.print(", ");
  Serial.print((long) jjj); Serial.print(", ");
  Serial.println((long) kkk);
}
