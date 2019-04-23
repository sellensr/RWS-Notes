version = "0.0.9"
version_info = "Emulate some of the functionality of an Arduino UNO for testing ideas."

# gets all of numpy but you will still need to refer to e.g. numpy.random.randn() as random.randn()
from numpy import *               # all of the array capable elements for numerical data          
from scipy.stats import norm      # comprehensive functions for Gaussian normal distributions
from datetime import *

t0Arduino = datetime.now()  # the time the simulated Arduino started running
A0 = 14                     # the pin numbers of each of the analog inputs on the UNO
A1 = 15
A2 = 16
A3 = 17
A4 = 18
A5 = 19

#some functions that do the same thing as these macros we defined
#define P Serial.print
#define PCS Serial.print(", ");Serial.print
#define PCSL Serial.print(", ");Serial.println
def P(x):
    print(x, end='', flush=True) # no space or end of line
def PCS(x):
    print(", ", end='', flush=True)
    print(x, end='', flush=True)
def PCSL(x):
    print(", ", end='', flush=True)
    print(x)
    
#def setup():                # these need to be defined in your sketch cell
#    print('in dummy setup')
#def loop():
#    print('in dummy loop')
    
def micros():               # microseconds since time was reset
    dt = datetime.now()-t0Arduino
    return dt.microseconds + dt.seconds * 1000000 

def delay(t):               # pause for t milliseconds
    tn = t0 = micros()
    while((tn-t0)/1000 <= t):
        tn = micros()
    
def delayMicroseconds(t):   # pause for t microseconds
    tn = t0 = micros()
    while((tn-t0) <= t):
        tn = micros()

def resetArduinoTime(): # run Arduino sketch cell code for t seconds
    global t0Arduino
    t0Arduino = datetime.now()    # restart the micros() clock

def runArduino(setup,loop,t = 300): # run Arduino sketch cell code for t seconds
    resetArduinoTime()
    setup()                       # run setup() once
    delay(1)
    while(micros() < t*1000000):  # until time runs out
        loop()                    # keep running loop() over and over
        delay(1)
    

PSEUDO_GROUND=   1.239
USB_VOLTAGE=     5.043
V33=             3.296
V11=             1.086

VrefX = USB_VOLTAGE

def analogRead(port,delay = 100):        #// a substitute for analogRead()
#   // Returns data as if there were various circuits attached to the Arduino
#   // with predefined events corresponding to some of the MECH 217 lab activities.
    t = micros()
    ts = t /1000000. #// time in seconds
    A = 0              #// so it can go a little negative and round back up to 0
    V = 0            #// voltage on the pin in volts
    if (port == A0):
        V = VTMP36(ts)#; // a TMP36 on a coffee cup, water poured at 10 seconds
    elif(port == A1):
        V = Vpot(ts)#;   // a potentiometer being turned manually through 180 degrees
    elif(port == A2):
        V = Vacc(ts)#;  // an accelerometer being held flat, then on each edge, then moving
    elif(port == A3):
        V = Vload(ts)#; // a load cell subject to increasing loads, then step loads
    elif(port == A4):
        V = Vpseudo(ts)#;  // the voltage of the amplifier pseudo-ground
    elif(port == A5):
        #V = sin(ts)*0.50 + 4.5; // a sin wave in case you need one
        V = Vtc(ts);
    else: V = 0
    A = int(1023 * V/VrefX + random.randn()*0.75)#;   // conversion bit variation
    A = min(1023,A)#;                                 // force into range
    A = max(0,A)
    delayMicroseconds(delay)  # Slow it down to reflect real time required fro Arduino ADC
    return A

def Vpseudo(ts):#{  // the voltage of the amplifier pseudo-ground
    return PSEUDO_GROUND + random.randn()*0.005

def VTMP36(ts, tc1 = 15, tc2 = 20): #{   // a TMP36 on a coffee cup, water poured at 10 seconds
    Ta = 22.
    Tc = 80.
    Ts = Ta
    tsu = int(ts)
    tw = ts - int(tsu/120)*120 - 10                # repeat every two minutes
    if(tw < 0): Ts = Ta                            # initially at ambient
    elif(tw < 60): Ts = Tc - exp(-tw/tc1)*(Tc-Ta)   # decay towards cup T
    else: Ts = Ta - exp(-(tw-60)/tc2)*(Ta-Tc)       # decay back to ambient
    return Ts * 0.010 + 0.5 + random.randn()*0.005 # voltage with 10mV noise

def Vtc(ts):
    return VTMP36(ts,tc1 = 4,tc2 = 8)

def Vpot(ts):     #{    // a potentiometer being turned manually through 180 degrees
    tw = ts - int(ts/20)*20 # repeat every 20 seconds
    if(tw < 3.1416): V = 0.8
    elif(tw < 2 * 3.1416): V = 0.8 + 1.50 * (cos(tw) + 1)
    elif(tw < 3 * 3.1416): V = 3.8
    elif(tw < 5 * 3.1416): V = 2.3 - 1.5 * sin(0.5*tw)
    else: V = 0.8
    return V + random.randn()*0.005;

def Vload(ts): #{     // a load cell subject to increasing loads, then step loads
    tsu = int(ts)
    tw = ts - int(tsu/60)*60                    # repeat every minute
    if(tw < 10): m = (tsu%60)/2                 # 0, 1, 2, 3, 4 kg
    elif(tw < 12): m = 1.5                      # back to 1.5, then jump to 2.5, 4.5, 0
    # frequency prop to m**-0.5, tau prop to m
    elif(tw < 25): m = 2.5 - exp(-(tw-12)/2.5)*cos((tw-12)*3.1416 *10/sqrt(2.5))
    elif(tw < 45): m = 4.5 - 2 * exp(-(tw-25)/4.5)*cos((tw-25)*3.1416 *10/sqrt(4.5))
    else: m = 0 - 4.5 * exp(-(tw-45)/.1)*cos((tw-45)*3.1416 *10/sqrt(.1))
    m += random.randn()*0.02
    return 1.24 + m * 0.287 + random.randn() * 0.005

def Vacc(ts):  #{   // an accelerometer being held flat, then on each edge, then moving
    tsu = int(ts)
    tw = ts - int(tsu/60)*60                         # repeat every minute
    if(tw < 5): m = 0
    elif(tw < 10): m = 1.0
    elif(tw < 15): m = -1.0
    elif(tw < 25): m = 0
    elif(tw < 35): m = 0.8 * sqrt(max(0,sin((tw-25)/10 * 3.1416)))*sin((tw-25)*3.1416)
    elif(tw < 40): m = 0
    elif(tw < 45): m = 2.8 * sqrt(sqrt(max(0,sin((tw-40)/5 * 3.1416))))*sin((tw-40)*5*3.1416)
    else: m = 0
    m += random.randn()*0.004
    return 1.57 + m * 0.487 + random.randn() * 0.005;

