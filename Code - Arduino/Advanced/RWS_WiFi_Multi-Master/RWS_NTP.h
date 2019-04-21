#ifndef _RWS_NTP_h  // avoid including multiple times
#define _RWS_NTP_h

#include <WiFiUdp.h>
#include <TimeLib.h>
#include "RWS_WiFi_Multi.h"

void sendNTPpacket(IPAddress& address);
time_t NTPsetup();
time_t getNtpTime();

void stampDate(String *ts,time_t t);
void stampTime(String *ts,time_t t);
void stampDateTime(String *ts,time_t t);

#endif //_RWS_NTP_h
