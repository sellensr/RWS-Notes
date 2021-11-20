// Track some variables to find out where the stack is and more about the memory map
#define M0_BASE   0x20000000
#define UNO_BASE  0x100
#define BASE M0_BASE

byte iByte0 = 42;
byte iByte1 = 42;
unsigned long uLong0 = 42;
unsigned long uLong1 = 42;
unsigned long uLong2 = 42;
unsigned long timeNow = 27;
byte iByte2 = 42;

int box[701] = {0};

void setup() {
  // put your setup code here, to run once:
  unsigned long dt = micros();
  static unsigned long timeLast = 0;
  timeLast = timeNow;
  timeNow = micros();
  Serial.begin(115200);
  while(!Serial && millis() < 10000);
  Serial.print("\n\n Stack Finding Test\n\n");
//  Serial.println(timeNow);
//  Serial.println(sizeof(timeNow));
  printAddr("timeNow", sizeof(timeNow),((unsigned long) &timeNow) - BASE);
  printAddr("dt", sizeof(dt),((unsigned long) &dt) - BASE);
  printAddr("iByte0", sizeof(iByte0),((unsigned long) &iByte0) - BASE);
  printAddr("iByte1", sizeof(iByte1),((unsigned long) &iByte1) - BASE);
  printAddr("iByte2", sizeof(iByte2),((unsigned long) &iByte2) - BASE);
  printAddr("uLong0", sizeof(uLong0),((unsigned long) &uLong0) - BASE);
  printAddr("uLong1", sizeof(uLong1),((unsigned long) &uLong1) - BASE);
  printAddr("uLong2", sizeof(uLong2),((unsigned long) &uLong2) - BASE);
  printAddr("timeLast", sizeof(timeLast),((unsigned long) &timeLast) - BASE);
  printAddr("box[0]", sizeof(box[0]),((unsigned long) &(box[0])) - BASE);
  printAddr("box[500]", sizeof(box[500]),((unsigned long) &(box[500])) - BASE);
  printAddr("box[700]", sizeof(box[700]),((unsigned long) &(box[700])) - BASE);
  Serial.println();
}

void printAddr(char *name, unsigned size, unsigned long addr){
  Serial.print(name); Serial.print(" has a size of ");
  Serial.print(size); Serial.print(" bytes and is located at 0x");
  Serial.print(addr,16); Serial.print(" (");
  Serial.print(addr); Serial.print(")");
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  int abc[] = {1,2,3,4,5,6,7,8,9,0};
  factorial((millis()/5000)%20, abc);
  delay(2000);
  Serial.print("\n");
}

unsigned long factorial(unsigned i, int abc[]){
  unsigned long ret = 0;
  static int depth = 0;
  depth++;
  Serial.print(depth);
  Serial.print(" deep in factorial(i) for i = "); Serial.print(i);
  delay(250);
  printAddr(", i ", sizeof(i),((unsigned long) &i) - BASE);
  delay(250);
  printAddr("     abc ", sizeof(abc),((unsigned long) &abc) - BASE);
  delay(250);
  printAddr("   depth ", sizeof(depth),((unsigned long) &depth) - BASE);
  delay(250);
  printAddr("     ret ", sizeof(ret),((unsigned long) &ret) - BASE);
  if(i < 2) ret = i;
  else ret = i * factorial(i-1,abc);
  depth--;
  return ret;
}
