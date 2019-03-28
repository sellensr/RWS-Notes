#include "RWS_WiFi_Multi.h"
// Example code for creating a WiFi connection with a known list
// Could be avoided with ESP8266WiFiMulti.h
/************************** Configuration ***********************************/

// defines MAX_SSID and NSSID, IO_USERNAME, IO_KEY, wlan_ssid[NSSID], wlan_pass[NSSID]
#include <ssid.h> // should include personal SSID data arrays like below, or insert right here,
// but use caution if you include private information in your source code
//#define MAX_SSID 10     // spaces to reserve in arrays, must be >= than NSSID
//int NSSID = 5;          // spaces initialized in arrays, could change
//const char *wlan_ssid[MAX_SSID] = {"KYC", "Signature", "KYC Harbour", "MITS Yard", "Signature5"};
//const char *wlan_pass[MAX_SSID] = {""   , "p2" , ""           , "p4" , "p5"  };

int ssidIndex = -1;   // index for the ssid list that matches a scan

void setup() {
  Serial.begin(115200);
  while (! Serial);
  WiFi_Multi_Init(wlan_ssid,wlan_pass,NSSID);
  WiFi.mode(WIFI_STA);  // station mode, not AP
  WiFi.disconnect();    // shut down any existing connection
}

void loop() {
  Serial.print("\nlistWiFiMine(): \n"); Serial.println(listWiFiMine());
  Serial.print("\nlistWiFiScan(): \n"); Serial.println(listWiFiScan());
  Serial.print("\nfindNextWiFi(0): "); Serial.println(findNextWiFi(0));
  Serial.print("findNextWiFi(3): "); Serial.println(findNextWiFi(3));
  Serial.print("findNextWiFi(8): "); Serial.println(findNextWiFi(8));
  Serial.print("existsWiFi(""test""): "); Serial.println(existsWiFi("test"));
  Serial.print("existsWiFi(""smwireless""): "); Serial.println(existsWiFi("smwireless"));
  Serial.print("existsWiFi(""SignatureOffshore""): "); Serial.println(existsWiFi("SignatureOffshore"));
  Serial.print("existsWiFi(""bob""): "); Serial.println(existsWiFi("bob"));
  Serial.print("existsWiFi(""Signature""): "); Serial.println(existsWiFi("Signature"));
  yield();
}
