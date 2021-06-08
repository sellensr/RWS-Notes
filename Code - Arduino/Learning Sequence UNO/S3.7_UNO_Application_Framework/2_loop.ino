void loop() {
  // Keep time in microseconds as an unsigned long for best accuracy.
  timeNow = micros();                    // the time we started this loop, microseconds
  // unsigned long can hold 0,1,2,...,4294967295, about 4 billion, a little more than 71 minutes of us
  timeNowS = timeNow / 1000000.;         // time now in seconds, float so it will have decimals
  // 32 bit floats have 6 or 7 significant figures and can reach +/- 3.4E38, so timeNowS is less precise 
  unsigned long dt = timeNow - timeLast; // the time difference since last loop, microseconds
  float dtS = dt / 1000000.;             // the same time difference in seconds
  loopTime = 0.9 * loopTime + 0.1 * dtS; // smooth the loopTime with a time constant of ten loops

  /* For each task in the list, check if it has been long enough since last running.
   *  If so, run the task and save the return value. Passing the task list index to the 
   *  task function allows it to access its own timing data, etc. Then record the loop 
   *  time for the latest run of the task function.
   */
  for(int i = 0;i < MAX_TASKS;i++){
    if((timeNow-lastTask[i]) / 1000000.0 >= dtTaskMin[i] || lastTask[i] == 0){
      unsigned long start = micros();
      lastTaskReturn[i] = (*fTask[i])(i);
      lastTaskRunTime[i] = micros()-start;
      lastTask[i] = timeNow;
    }
  }

  timeLast = timeNow;      // save the old value for next time through the loop
}
