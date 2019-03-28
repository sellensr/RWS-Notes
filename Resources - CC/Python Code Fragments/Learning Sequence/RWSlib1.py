version = "0.0.9"
version_info = "Just a little test library with some MECH 217 functions in it."
import numpy as np

def movingAverage(T,backward = 0,forward = 0):
    # return a moving averaged version of T that includes 
    # up to backward samples from before the current point
    # and up to forward samples from after the current point
    # Tails of the sample will be averaged using fewer points.
    Tma = T.copy()                   # a copy to store the averaged values in
    for i in range(0,len(T)): 
        s = max(i-backward,0)        # start at 0 or backward
        f = min(i+forward+1,len(T))  # finish at forward or end of data
        Tma[i] = np.mean(T[s:f])
    return Tma

def expSmooth(t,T,tau):        # returns a smoothed version of T with time constant tau
    Ts = T.copy()              # another vector for the smoothed data
    for i in range(1,len(Ts)): # here's where the actual smoothing happens
        dt = t[i]-t[i-1]
        w = min(dt/tau,1)
        Ts[i] = Ts[i-1] * (1-w) + T[i] * w
    return Ts                  # return the smoothed data

 