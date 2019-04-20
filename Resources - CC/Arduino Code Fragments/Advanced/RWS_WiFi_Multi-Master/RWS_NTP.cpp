/*
Assumes a working WiFi connection
Adapted by Rick Sellens 2019-04-20 from:

 TimeNTP example at https://github.com/PaulStoffregen/Time
 */

#include "RWS_NTP.h"

/* Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead */
IPAddress timeServer;                         // time.nist.gov NTP server address, class passes by reference
//const char* nistHost = "time.nist.gov";     // Round-robin DAYTIME protocol
//const char* ntpServerName = "time.nist.gov";
const char* ntpServerName = "pool.ntp.org";   // may be more available than nist
unsigned int localPort = 2390;                // local port to listen for UDP packets
WiFiUDP Udp;                                  // a UDP instance to send/receive the packets
const int timeZone = 0;                       // UTC

time_t NTPsetup(){
  Udp.begin(localPort);
  WiFi.hostByName(ntpServerName, timeServer); // both passed by reference
  setSyncProvider(getNtpTime);
  setSyncInterval((time_t) SECS_PER_DAY);
  time_t ut = getNtpTime();                     
  setTime(ut);                                // init the Time library 
  return ut;
}

void stampDateTime(String *ts,time_t t)
{   // adds an ISO YYYY-MM-DD hh:mm:ss date and time stamp onto the string, uses now() if t = 0
  stampDate(ts,t);
  *ts += " ";
  stampTime(ts,t);
}

void stampDate(String *ts,time_t t)
{   // adds an ISO YYYY-MM-DD date stamp onto the string, uses now() if t = 0
  if(t == (time_t) 0) t = now();
  unsigned y,mn,d;
  y = year(t); mn = month(t); d = day(t);
  *ts += (String) y; *ts += "-";
  if(mn < 10) *ts += "0"; *ts += (String) mn; *ts += "-";
  if(d < 10) *ts += "0"; *ts += (String) d;    
}

void stampTime(String *ts,time_t t)
{   // adds an hh:mm:ss time stamp onto the string, uses now() if t = 0
  if(t == (time_t) 0) t = now();
  unsigned h,m,s;
  h = hour(t); m = minute(t); s = second(t);
  if(h < 10) *ts += "0"; *ts += (String) h; *ts += ":";
  if(m < 10) *ts += "0"; *ts += (String) m; *ts += ":";
  if(s < 10) *ts += "0"; *ts += (String) s; 
}

// Copied from the Time library NTP example 2019-04-20
// Debug code commented out, wait time raised to 2000
/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  //Serial.println("Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 2000) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      //Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  //Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}
