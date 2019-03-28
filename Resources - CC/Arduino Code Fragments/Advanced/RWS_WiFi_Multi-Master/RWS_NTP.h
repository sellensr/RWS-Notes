#ifndef _RWS_NTP_h  // avoid including multiple times
#define _RWS_NTP_h

#include <WiFiUdp.h>
#include <TimeLib.h>
#include "RWS_WiFi_Multi.h"

unsigned long sendNTPpacket(IPAddress& address);
int NTPsetup();
time_t getNtpTime();
void stampNtpTime(String *ts,time_t t);

#endif //_RWS_NTP_h
