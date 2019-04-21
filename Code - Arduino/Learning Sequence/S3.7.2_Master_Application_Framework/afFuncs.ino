// These tasks probably won't change from one application to another.


// This task can be used to fill out the task list
int emptyTask(int iTask){    // do nothing in this task
  // iTask is the Task index so the task knows 
  return 0; // status code from task, 0 for nothing unusual
}

// This task can be used to move a servo around to help with calibration and with testing
int servoExerciseTask(int iTask){    // return the current target location for the servo
  // iTask is the Task index so the task knows 
  return 0; // status code from task, 0 for nothing unusual
}

// This task will be useful in any application that reads from analog inputs
int readTask(int iTask){  // read and smooth current analog values -- call this task first or second
  static float vConversion = AREF / (ANALOG_MAX + 1.0); // Calculate only once for speed
  float dt = timeNow-lastTask[iTask];                   // time since last reading in microseconds
  for(int i = 0;i<NCH;i++){
    a[i] = analogRead(A0+i);                      // assumes analog channel numbers are contiguous
    float w = min(1.,dt / (tau[i] * 1000000.0));  // weight to give the latest reading of a in smoothing
    as[i] = w * a[i] + (1-w) * as[i];             // weighted average will smooth the data to reduce noise
    v[i] = (a[i] + 0.5) * vConversion;            // convert to voltages
    vs[i] = (as[i] + 0.5) * vConversion;
  }
  calcReadResults();
  return 0; // status code from task, 0 for nothing unusual
}

// This task will be useful in any application that reads from digital inputs
int digTask(int iTask){  // read current digital values -- call this task first or second
  for(int i = 0;i<NCHD;i++) d[i] = digitalRead(i);
  return 0; // status code from task, 0 for nothing unusual
}

int showTimesTask(int iTask){
  P("\nBytes of free memory: "); P(bytesFree());
  P("\nLatest / worst ever execution times for each task in the list [microseconds]\n");
  P(timeNowS,3);             // time the loop started
  for(int i = 0;i<MAX_TASKS;i++){
    PCS(lastTaskRT[i]);
  }
  P("\n");
  P(timeNowS,3);             // time the loop started
  for(int i = 0;i<MAX_TASKS;i++){
    PCS(worstTaskRT[i]);
  }
  P("\n\n");
  return 0; // status code from task, 0 for nothing unusual
}

int blinkPinTask(int i){  // changes the status of the onboard LED on each call
  if(digitalRead(BLINK_PIN)) digitalWrite(BLINK_PIN,LOW);
  else digitalWrite(BLINK_PIN,HIGH);
  return 0;
}
