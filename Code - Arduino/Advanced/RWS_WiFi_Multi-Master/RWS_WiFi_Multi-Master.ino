/*
 * To Do: secure wifi client connections for WiFi 101
 *        save manually entered networks from AP?
 *        detect and recover from crashes / exceptions
 *        is there a future in supporting others besides ESP32?
 *        or even supporting the ESP32 if Particle provides secure IOT?
 */
 
/* RWS_WiFi_Multi provides cross-platform facilities for tracking and using
    multiple different wireless networks, logging into whichever is currently
    available. The implementations on the ESP processors are just different
    enough from WiFi101 that they require some helper code.

    This library also allows you to easily keep your credentials separated
    from your project source code, so you don't accidentally give away
    your passwords and keys while sharing your code.

    Alternatives are ESP8266WiFiMulti.h and WiFiMulti.h for the ESP family.

    To install this library, copy the entire folder to your Arduino libraries
    folder (probably Documents/Arduino/libraries), then rename that folder
    to RWS_WiFi_Multi so you don't mistake it for the master copy.

    2018-11-25: Works with ESP8266 on FTDI, ESP8266 Feather, M0 WINC1500 Feather,
    and ESP32 Feather. Added wlan_allow[] to ssid.h for switching out of the list.
    WiFi_Multi_Init() now determines number of nets in the list independent of NSSID.

    2018-12-19: Adding secure connections and AP for net selection. Both are causing 
    flakiness problems with M0 Feather, which still works with insecure, list modes.
    ESP32 and ESP8266 Feathers working well. 
*/

//#define USE_WIFI_SECURE

#include "Root_Certs.h"
#include "RWS_WiFi_Multi.h"
#include "RWS_NTP.h"

// ssid.h defines MAX_SSID and NSSID, IO_USERNAME, IO_KEY, wlan_ssid[], wlan_pass[], wlan_allow[]
// You should create your own copy somewhere that the IDE can find it, probably something like
// sellensr/Documents/Arduino/libraries/RWS/ssid.h
#include <ssid.h> // It should include personal SSID data arrays like below, or insert right here,
// but use caution if you include private information in your source code!!!!!
//#define MAX_SSID 10     // spaces to reserve in arrays, must be >= than NSSID
//int NSSID = 5;          // spaces initialized in arrays, could change
// The last ssid in the list must be set to "" as a delimiter. 
// user is only non-blank for enterprise networks. Be extra cautious about including enterprise passwords.
//char *wlan_ssid[MAX_SSID] = {"KYC", "Signature", "KYC Harbour", "MITS Yard", "Signature5",""};
//char *wlan_user[MAX_SSID] = {""   , "u2"       , ""           , ""         , "u5"        ,""};
//char *wlan_pass[MAX_SSID] = {""   , "p2"       , ""           , "p4"       , "p5"        ,""};
//byte wlan_allow[MAX_SSID] = {1    , 1          , 1            , 1          , 0           ,0 };

//int ssidIndex = -1;                 // index for the ssid list that matches a scan
#define FROM_UTC -5                 // hours to add to UTC for local
char ssidAP[] = "RWS_WiFi_Multi";   // Name of the access point we will create, up to 32 characters
char passAP[] = "rickwifi";         // password for the AP -- make it long enough Apple will join (8 chars)


void setup() {
  Serial.begin(115200);   while (! Serial)  yield();
  delay(1000);
  Serial.print("\n\nRWS_WiFi_Multi Example\n\n");
  
  // Initialize the pointer arrays from user data. User data must remain correct.
  WiFi_Multi_Init(wlan_ssid, wlan_user, wlan_pass, wlan_allow, MAX_SSID);
  WiFi.disconnect();

  //WiFi_Multi_AP(ssidAP,passAP,2000);  // only uncomment to test AP mode

  if(WiFi.status() != WL_CONNECTED){  // if that didn't work, try this cascade
    Serial.print("Trying WiFi_Multi_Memory() to connect to previously used ssid...\n");
    if(WiFi_Multi_Memory(20) != WL_CONNECTED){
      Serial.println("No Previous Connection Made...");
      Serial.print("Trying WiFi_Multi_List() to connect to ssid from ssid.h...\n");
      if(WiFi_Multi_List(20) != WL_CONNECTED){
        Serial.println("No List Connection Made...");
        Serial.print("Trying WiFi_Multi_AP() to select via access point...\n");
        WiFi_Multi_AP(ssidAP,passAP,200);
      }
    }
  }

  if (WiFi.status() == WL_CONNECTED) {     // There is a network
    Serial.print("WiFi connected to IP address: ");
    // Coercing it explicitly to an IPAddress will format correctly in multiple implementations
    IPAddress ip = WiFi.localIP();
    Serial.print(ip);
    Serial.print(" on SSID: ");
    Serial.println(WiFi.SSID());
    time_t ut = NTPsetup();   // initialize the time from the NTP server
    Serial.print("NTPsetup returns: "); Serial.println(ut);
  } else Serial.print("\nTried everything, but there's no wifi connection... Sorry\n\n");
}

void loop() {
  int i = 0;
  String dateTimeStamp = "";
  String timeOnlyStamp = "";

  String stamp = "";
  time_t utLOC = now();
  time_t utNTP = getNtpTime();
  //while(utNTP == 0) utNTP = getNTPtime();
  Serial.print("LOC: "); stampDateTime(&stamp,utLOC); Serial.print(stamp); Serial.print("  ");
  stamp = "";
  Serial.print("NTP: "); stampTime(&stamp,utNTP); Serial.print(stamp); Serial.print("\n");


  // refresh and print the time stamp every time
  time_t ut = now() + FROM_UTC * 3600L;
  stampDateTime(&dateTimeStamp, ut);
  stampTime(&timeOnlyStamp, ut);
  Serial.print("\n"); Serial.print(dateTimeStamp); delay(1000); Serial.print("   "); Serial.println(timeOnlyStamp);
  
  #ifdef USE_WIFI_SECURE
    #if defined(ESP32) || defined(ESP8266)
      // WiFiClientSecure and client.connect() is for ESP32 and ESP 8266
      // Need to client.setCACert() for each host by hand for ESP32, but ESP8266 seems to have some installed.
      WiFiClientSecure client;
      //client.setCACert(howsmyssl_root_ca);    // needs to be set properly before attempting to connect to host
      //client.setCertificate(test_client_key); // for client verification
      //client.setPrivateKey(test_client_cert); // for client verification
    #elif defined(WINC1500) // Feather M0 WINC1500
      // WiFi101 uses WiFiClient and client.connectSSL()
      // Manages root ca certificates itself on the winc1500 (I think)
      WiFiClient client;
    #endif
    int port = 443;
  #else
    // this is generic for insecure clients
    WiFiClient client;
    int port = 80;
  #endif

  // Visit some web pages to confirm connections work
  for(int i = 0;*hosts[i] != 0;i++){        // go through the list of test hosts
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\n\nConnecting to "); Serial.println(hosts[i]);
      #ifdef USE_WIFI_SECURE
        #ifdef ESP32
          client.setCACert(root_cas[i]);        // set the correct certificate for the host
        #endif
      #endif
      #if defined(USE_WIFI_SECURE) && defined(WINC1500)
      if (!client.connectSSL(hosts[i], port)) {
      #else
      if (!client.connect(hosts[i], port)) {
      #endif
        Serial.println("connection failed, wait 5 sec...");
      } else {
        client.print(requests[i]);          // send the appropriate request, then print returns
        String line = client.readStringUntil('\n');
        Serial.println(line);
        unsigned long cnt = 0;              // total characters
        unsigned long cntLine = 0;          // total characters on this line
        while (client.available()) {
          char c = client.read();
          if(cnt < 2000 && cntLine < 80){ 
            Serial.write(c);
            yield();
            delay(5);
          }
          cnt++;
          cntLine++;
          if(c == '\n'){
            if(cntLine >= 80) Serial.write(c);
            cntLine = 0;
          }
        }
        Serial.print("\n...closing connection\n");
        client.stop();
        Serial.print("wait 5 sec...\n");
      }
      delay(5000);
    }
  }
  
  // Compare my network list and those visible locally
  Serial.print("\n\nlistWiFiMine(&Serial): \n"); i = listWiFiMine(&Serial);
  Serial.print("Total Count: "); Serial.println(i);
  Serial.print("\n\nlistWiFiScan(&Serial): \n"); i = listWiFiScan(&Serial);
  Serial.print("Total Count: "); Serial.println(i);

  // Demonstrate findNextWiFi() functionality
  Serial.print("\n\nfindNextWiFi(0): "); Serial.println(findNextWiFi(0));
  Serial.print("findNextWiFi(3): "); Serial.println(findNextWiFi(3));
  Serial.print("findNextWiFi(8): "); Serial.println(findNextWiFi(8));

  // Demonstrate existsWiFi() functionality
  Serial.print("\n\nexistsWiFi(""test""): "); Serial.println(existsWiFi("test"));
  Serial.print("existsWiFi(""smwireless""): "); Serial.println(existsWiFi("smwireless"));
  Serial.print("existsWiFi(""SignatureOffshore""): "); Serial.println(existsWiFi("SignatureOffshore"));
  Serial.print("existsWiFi(""bob""): "); Serial.println(existsWiFi("bob"));
  Serial.print("existsWiFi(""Signature""): "); Serial.println(existsWiFi("Signature"));

  yield();  // let priority programming get through the system, especially on the 8266
}
