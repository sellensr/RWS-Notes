#ifndef _RWS_WiFi_Multi_h  // avoid including multiple times
#define _RWS_WiFi_Multi_h

#if defined(ESP32) || defined(ESP8266)
  #define Serial_ HardwareSerial
#endif

#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD) // Feather M0 WINC1500
  #define WINC1500
  #include <SPI.h>
  #include <WiFi101.h>
#elif defined(ESP8266)   // ESP8266
  #include <ESP8266WiFi.h>
  #include <WiFiClientSecure.h>
#else                    // ESP32 and hopefully others
  #include <WiFi.h>
  #include <WiFiClientSecure.h>
#endif

#ifdef ESP32  // Because they can't all agree on names of encryption types
//https://www.arduino.cc/en/Reference/WiFiEncryptionType 
//says WiFi.encryptionType() returns (seems to match WiFi101)
//TKIP (WPA) = 2
//WEP = 5
//CCMP (WPA) = 4
//NONE = 7
//AUTO = 8
//https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/scan-class.html says
//WiFi.encryptionType returns a number that encodes encryption type as follows: 
//* 5 : ENC_TYPE_WEP - WEP * 2 : ENC_TYPE_TKIP - WPA / PSK * 4 : ENC_TYPE_CCMP - WPA2 / PSK 
//* 7 : ENC_TYPE_NONE - open network * 8 : ENC_TYPE_AUTO - WPA / WPA2 / PSK
//which seems to be the same as WiFi101

// From esp_wifi_types.h in the esp32 source come these names staring with WIFI_:
#define ENC_TYPE_NONE         WIFI_AUTH_OPEN
#define ENC_TYPE_WEP          WIFI_AUTH_WEP
#define ENC_TYPE_TKIP         WIFI_AUTH_WPA_PSK
#define ENC_TYPE_CCMP         WIFI_AUTH_WPA2_PSK
#define ENC_TYPE_AUTO         WIFI_AUTH_WPA_WPA2_PSK
// I'm just making up an ENC format name for the enterprise encryption
#define ENC_TYPE_ENTERPRISE   WIFI_AUTH_WPA2_ENTERPRISE  /**< authenticate mode : WPA2_ENTERPRISE */
#else
#define ENC_TYPE_ENTERPRISE   999 // a made up value that WIFI101 won't be returning
#endif

int WiFi_Multi_Init(char **s, char **u, char **p, byte *a, unsigned m);
int WiFi_Multi_Memory(unsigned t);
int WiFi_Multi_List(unsigned t);
int WiFi_Multi_AP(char *ssidAP, char *passAP, unsigned t);
int listWiFiMine(Serial_ *port);
int listWiFiScan(Serial_ *port);
int existsWiFi(char* ssid);
int findNextWiFi(int start);

#endif //_RWS_WiFi_Multi_h
