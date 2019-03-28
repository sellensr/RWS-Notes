// These tasks are defined here since they will probably change for each application.
// There are some other task functions that won't be as likely to change in the afFuncs tab
// Comment out the parts you don't want to see, or this list will get very long!
int consoleTask(int iTask){  // report to the serial monitor -- call this task last
  P(timeNowS,3);             // time the loop started
  PCS(1/loopTime,0);         // typical looping rate in Hz
  for(int i = 0;i<NCH;i++){  // the collected data from the analog channels
//    PCS(a[i]);               // raw
//    PCS(as[i]);              // smoothed
//    PCS(v[i],4);             // as voltage
//    PCS(vs[i],4);
//    PCS(x[i],4);             // as a followon result, e.g. temperature
    PCS(xs[i],4);
  }
  PCS(temp);
  PCS(hum);
  PCS(pres);
  P("\n");
  return 0; // status code from task, 0 for nothing unusual
}

// This conversion is defined here since it will probably change for each application.
void calcReadResults(){           // called by readTask to do conversions from inputs
  for(int i = 0;i<NCH;i++){       // default to voltage
    x[i] = v[i];
    xs[i] = vs[i];
  }
  x[0] = (v[0] - 0.5) * 100.0;    // assume a TMP36 on A0 -- customize others to match app
  xs[0] = (vs[0] - 0.5) * 100.0;
}
