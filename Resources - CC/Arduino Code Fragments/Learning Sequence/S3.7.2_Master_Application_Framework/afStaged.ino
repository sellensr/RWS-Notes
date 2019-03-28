/* Long tasks may need to be broken up into stages to make sure all the other tasks have a chance 
 *  to execute promptly. This would be especially true for tasks with a sequence like this:
 *      Tell a servo to start moving and block any further instructions (stage 0)
 *      Wait at least 10 ms then update position until close (stage 1)
 *      Make detailed measurements of final approach, then re-enable further instructions (stage 2)
 *    
 *  This example sums a series and the unstaged version is much simpler than the staged version
 *  although it takes about 30 ms to execute. That could be too long between executions for the 
 *  other tasks and would encourage us to break it down into manageable stages.
 */
int unstagedTask(int iTask){  // this task takes a long time to sum a series of 30000 values
  unsigned long sum = 0;
  for(int i=0;i<30000;i++) sum += i;
  P(timeNowS,3);
  PCS("In unstagedTask(), sum");
  PCSL(sum);
  return 0;
}

int stagedTask(int iTask){  // use this strategy to spread out a big task over multiple loops
  static unsigned long sum = 0;
  static unsigned long lastStage0 = 0, lastStage1 = 0, lastStage2 = 0, dtTaskSaved = 0;
  int thisStage = lastTaskReturn[iTask];        // depends on what was saved last time through
  int nextStage = thisStage;                    // repeat the same stage next time unless progress happens
  switch(thisStage){                            // cases for each stage of the process
    case 0:                                     // nothing pending, so start over
      dtTaskSaved = dtTaskMin[iTask];           // Speed up the repetition (optional)
      dtTaskMin[iTask] = 0.003;                      
      sum = 0;                                  // Do stage 0 (just a simple sum for an example)
      for(int i=0;i<10000;i++) sum += i;
      lastStage0 = micros();                    // Save the completion time
      nextStage = 1;                            // Move on to stage 1 next time
      break;
    case 1:                                     // Stage 0 already completed
      if(micros()-lastStage0 > 10000){          // Make sure we waited long enough (optional)
        for(int i=10000;i<20000;i++) sum += i;  // Do stage 1 (more of the series)
        lastStage1 = micros();                  // Save the completion time
        nextStage = 2;                          // Move on to stage 2 next time
      }
      else nextStage = 1;                       // Stay at stage 1 if need more time (redundant, no progress happened)
      break;
    case 2:
      for(int i=20000;i<30000;i++) sum += i;    // Do Stage 2 (the rest of the series)
      lastStage2 = micros();
      nextStage = 0;                            // All done, so go back to stage 0, nothing pending
      dtTaskMin[iTask] = dtTaskSaved;           // Restore the old repetition rate                      
      break;
    default:
      break;
  }
  P(timeNowS,3);
  PCS("In stagedTask(), thisStage, nextStage, sum");
  PCS(thisStage);
  PCS(nextStage);
  PCSL(sum);
  return nextStage; // stage to run next time, 0 for nothing pending
}
