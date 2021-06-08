#include "af.h"
#define NCH 3   

// Global Variables are available throughout the code and don't lose their values like locals
unsigned long timeNow = 0;          // the time we started the current loop(), microseconds
float timeNowS = 0.0;               // timeNow, but in seconds, float so it will have decimals
unsigned long timeLast = 0;         // timeNow, the last time we went through the loop(), microseconds

unsigned a[NCH];

// Make the subtasks a little more flexible by passing them an argument
// and having them return a status code -- this will be useful later
int readTask(int iTask){  // read current analog values -- call this task first
  for(int i = 0;i<NCH;i++) a[i] = analogRead(A0+i); 
  return 0; // status code from task, 0 for nothing unusual
}

int consoleTask(int iTask){  // report to the serial monitor -- call this task last
  P(timeNowS,3);             // time the loop started
  for(int i = 0;i<NCH;i++){ 
    PCS(a[i]);
  }
  P("\n");
  return 0; // status code from task, 0 for nothing unusual
}

/* These two declarations define task1 and task2 as pointers to functions
 *  that return an integer and take a single integer argument. They take the 
 *  same form as function declarations, except with (*task1) in place of the 
 *  function name. The parentheses make it clear that *task1 should be a unit
 *  and *task1 refers to the thing pointed to by the pointer task1.
 */
int (*task1)(int);
int (*task2)(int);

/* This declaration defines fTask as an array of pointers to functions
 *  that return an integer and take a single integer argument. Thus it is 
 *  a list of pointers like task1 and task 2, and we can iterate on that list.
 */
#define MAX_TASKS 2
int (*fTask[MAX_TASKS])(int);       // pointers to the functions that do each task

void setup() {
  Serial.begin(115200);         // set the serial port speed
  P("\n\nRick's Applicatio Framework S3.7.1 Pointers to Functions\n\n");
  P("Time [s], A0, A1, ...\n"); // provide headings to name the outputs
  fTask[0] = task1 = readTask;  // set the first element of fTask[] to point to the function readTask()
  fTask[1] = task2 = consoleTask;
}

// The loop() function is much smaller when the tasks are broken out into 
// separate functions, like those pointed to by task1() and task2() here.
void loop() {
  timeNow = micros();                    // the time we started this loop, microseconds
  timeNowS = timeNow / 1000000.;         // time now in seconds, float so it will have decimals
  
//  task1(1); // call readTask() by using the pointer, so far just more complicated
//  task2(2);

  for(int i = 0;i<MAX_TASKS;i++) // same result by iterating down the list
    (fTask[i])(i+1);             // allows the same code to interact with multiple task functions
  
  timeLast = timeNow;      // save the old value for next time through the loop
}
