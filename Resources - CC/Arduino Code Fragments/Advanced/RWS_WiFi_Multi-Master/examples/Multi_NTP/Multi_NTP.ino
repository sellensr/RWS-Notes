// This is a direct copy from the RWS_WiFi_Multi-Master tab in the master library code.

/* RWS_WiFi_Multi provides cross-platform facilities for tracking and using 
 *  multiple different wireless networks, logging into whichever is currently 
 *  available. The implementations on the ESP processors are just different 
 *  enough from WiFi101 that they require some helper code. 
 *  
 *  This library also allows you to easily keep your credentials separated 
 *  from your project source code, so you don't accidentally give away
 *  your passwords and keys while sharing your code.
 *  
 *  Alternatives are ESP8266WiFiMulti.h and WiFiMulti.h for the ESP family.
 *  
 *  To install this library, copy the entire folder to your Arduino libraries 
 *  folder (probably Documents/Arduino/libraries), then rename that folder 
 *  to RWS_WiFi_Multi so you don't mistake it for the master copy.
 *  
 *  2018-11-25: Works with ESP8266 on FTDI, ESP8266 Feather, M0 WINC1500 Feather,
 *  and ESP32 Feather. Added wlan_allow[] to ssid.h for switching out of the list.
 *  WiFi_Multi_Init() now determines number of nets in the list independent of NSSID.
 */ 

#include "RWS_WiFi_Multi.h"
#include "RWS_NTP.h"

// ssid.h defines MAX_SSID and NSSID, IO_USERNAME, IO_KEY, wlan_ssid[], wlan_pass[], wlan_allow[] 
// You should create your own copy somewhere that the IDE can find it, probably something like
// sellensr/Documents/Arduino/libraries/RWS/ssid.h                  
#include <ssid.h> // It should include personal SSID data arrays like below, or insert right here,
                  // but use caution if you include private information in your source code
//#define MAX_SSID 10     // spaces to reserve in arrays, must be >= than NSSID
//int NSSID = 5;          // spaces initialized in arrays, could change
//char *wlan_ssid[MAX_SSID] = {"KYC", "Signature", "KYC Harbour", "MITS Yard", "Signature5",""};
//char *wlan_pass[MAX_SSID] = {""   , "p2"       , ""           , "p4"       , "p5"        ,""};
//byte wlan_allow[MAX_SSID] = {1    , 1          , 1            , 1          , 0           ,0 };

int ssidIndex = -1;     // index for the ssid list that matches a scan
#define FROM_UTC -5     // hours to add to UTC for local

void setup() {
  Serial.begin(115200);   while (! Serial);
  Serial.print("\n\nRWS_WiFi_Multi Example\n\n");

  // Initialize the pointer arrays from user data. User data must remain correct.
  WiFi_Multi_Init(wlan_ssid,wlan_pass,wlan_allow);
  // Find the first available network connection
  ssidIndex = findNextWiFi(0);

  if(ssidIndex >= 0){       // There is a matching network
    WiFi.disconnect();      // shut down any existing connection
    while(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(wlan_ssid[ssidIndex]);
      WiFi.begin(wlan_ssid[ssidIndex],wlan_pass[ssidIndex]);
      unsigned long ts = millis();
      // wait until connected or timed out before trying again
      while(millis()-ts < 10000 && WiFi.status() != WL_CONNECTED);
    }
    Serial.print("WiFi connected to IP address: ");
    // Coercing it explicitly to an IPAddress will format correctly in multiple implementations
    IPAddress ip = WiFi.localIP();
    Serial.print(ip);
    Serial.print(" on SSID: ");
    Serial.println(WiFi.SSID()); 
    NTPsetup();   // initialize the time from the NTP server
  }
}

void loop() {
  int i = 0, port = 80;
  String timeStamp = "";
  WiFiClient client;
  char *host = "cbc.ca";

  // refresh and print the time stamp every time
  stampNtpTime(&timeStamp,now()+FROM_UTC * 3600L); 
  Serial.print(timeStamp); Serial.print(" ");
  
  if(WiFi.status() == WL_CONNECTED){
    Serial.print("connecting to "); Serial.println(host);
    if (!client.connect(host, port)) {
      Serial.println("connection failed, wait 5 sec...");
    } else {
      client.print("GET HTTP/1.1\r\n\r\n");
      //read back one line from server just to prove it connected
      String line = client.readStringUntil('\r');
      Serial.println(line);
      Serial.println("closing connection");
      client.stop();
      Serial.println("wait 5 sec...");
    }
    delay(5000);
  }

  // Compare my network list and those visible locally
  Serial.print("\nlistWiFiMine(): \n"); i = listWiFiMine();
  Serial.print("Total Count: "); Serial.println(i);
  Serial.print("\nlistWiFiScan(): \n"); i = listWiFiScan();
  Serial.print("Total Count: "); Serial.println(i);

  // Demonstrate findNextWiFi() functionality
  Serial.print("\nfindNextWiFi(0): "); Serial.println(findNextWiFi(0));
  Serial.print("findNextWiFi(3): "); Serial.println(findNextWiFi(3));
  Serial.print("findNextWiFi(8): "); Serial.println(findNextWiFi(8));

  // Demonstrate existsWiFi() functionality
  Serial.print("existsWiFi(""test""): "); Serial.println(existsWiFi("test"));
  Serial.print("existsWiFi(""smwireless""): "); Serial.println(existsWiFi("smwireless"));
  Serial.print("existsWiFi(""SignatureOffshore""): "); Serial.println(existsWiFi("SignatureOffshore"));
  Serial.print("existsWiFi(""bob""): "); Serial.println(existsWiFi("bob"));
  Serial.print("existsWiFi(""Signature""): "); Serial.println(existsWiFi("Signature"));

  yield();  // let priority programming get through the system, especially on the 8266
}
