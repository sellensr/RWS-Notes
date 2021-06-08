void loop() {
  // Update all the loop timing information.
  // Keep time in microseconds as an unsigned long for best accuracy.
  timeNow = micros();                    // the time we started this loop, microseconds
  // unsigned long can hold 0,1,2,...,4294967295, about 4 billion, a little more than 71 minutes of us
  timeNowS = timeNow / 1000000.;         // time now in seconds, float so it will have decimals
  // 32 bit floats have 6 or 7 significant figures and can reach +/- 3.4E38, so timeNowS is less precise 
  dt = timeNow - timeLast; // the time difference since last loop, microseconds
  dtS = dt / 1000000.;     // the same time difference in seconds
  // smooth the loopTime with a time constant of ten loops
  loopTime = 0.9 * loopTime + 0.1 * dtS;

  /* For each task in the list, check if it has been long enough since last running.
   *  If so, run the task and save the return value. Passing the task list index to the 
   *  task function allows it to access its own timing data, etc. 
   *  
   *  Then record the loop time for the latest run of the task function.
   *  
   */
  for(int i = 0;i < MAX_TASKS;i++){
    float sWaited = (timeNow-lastTask[i]) / 1000000.0;
    if(sWaited >= dtTaskMin[i] || lastTask[i] == 0){
      unsigned long start = micros();
      lastTaskReturn[i] = (*fTask[i])(i);
      lastTaskRT[i] = micros()-start;
      // don't count the first few loops in the worst case times -- initialization
      if(lastTaskRT[i] > worstTaskRT[i] && loopCount > MAX_TASKS) 
        worstTaskRT[i] = lastTaskRT[i];
      lastTask[i] = timeNow;
    }
    // Get back to the top of the loop promptly, even if it means not finishing 
    // everything in the list. Priority tasks need to be at the top of the list.
    if((micros()-timeNow) > skipTimeS * 1000000.){
      P("Breaking after task ");
      P(i); P("\n");
      break;
    }
  }

  timeLast = timeNow;      // save the old value for next time through the loop
  loopCount++;
}
