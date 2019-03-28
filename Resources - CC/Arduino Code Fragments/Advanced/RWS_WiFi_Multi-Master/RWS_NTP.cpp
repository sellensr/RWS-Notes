/*
Assumes a working WiFi connection
Adapted by Rick Sellens 2016-02-06 from:

 Udp NTP Client https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/NTPClient/NTPClient.ino

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 updated for the ESP8266 12 Apr 2015 
 by Ivan Grokhotkov

 This code is in the public domain.

 */

#include "RWS_NTP.h"

/* Don't hardwire the IP address or we won't get the benefits of the pool.
 *  Lookup the IP address for the host name instead */
IPAddress timeServerIP;                       // time.nist.gov NTP server address
//const char* nistHost = "time.nist.gov";     // Round-robin DAYTIME protocol
//const char* ntpServerName = "time.nist.gov";
const char* ntpServerName = "pool.ntp.org";   // may be more available than nist
unsigned int localPort = 2390;        // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48;       // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE];  // buffer to hold incoming and outgoing packets
WiFiUDP udp;                          // a UDP instance to send/receive the packets

int NTPsetup(){
  udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval((time_t) SECS_PER_DAY);
  return 0;
}

time_t getNtpTime()
{ // returns the number of seconds since Jan 1 1970, or 0 on failure (unix epoch)
  // call often enough to avoid lost time due to interrupts
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServerIP);    // send an NTP packet to a time server
  //Serial.print("NTP request sent... ");
  unsigned long NTPmillis = millis() - 1000;    // approximate adjustment for network latency
  delay(2000);        // wait to see if a reply is available
  int cb = udp.parsePacket();
  if (!cb) {
    //Serial.println("no packet yet");
    return 0;
  }
  else {
    //Serial.print("packet received, length=");
    //Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, extract the two words:
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900) - 70 years of seconds = unix epoch:
    time_t NTPlast = (highWord << 16 | lowWord) - 2208988800UL;
    setTime(NTPlast + (millis()-NTPmillis)/1000);   // adjust for elapsed time
    return NTPlast + (millis()-NTPmillis)/1000;
  }
}

void stampNtpTime(String *ts,time_t t)
{   // puts a time stamp into the string, uses now() if t = 0
  if(t == (time_t) 0) t = now();
  unsigned h,m,s,y,mn,d;
  h = hour(t); m = minute(t); s = second(t); y = year(t); mn = month(t); d = day(t);
  *ts = (String) y; *ts += "-";
  if(mn < 10) *ts += "0"; *ts += (String) mn; *ts += "-";
  if(d < 10) *ts += "0"; *ts += (String) d; *ts += " ";    
  if(h < 10) *ts += "0"; *ts += (String) h; *ts += ":";
  if(m < 10) *ts += "0"; *ts += (String) m; *ts += ":";
  if(s < 10) *ts += "0"; *ts += (String) s; 
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
//  Serial.println("sending NTP packet...");
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
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
