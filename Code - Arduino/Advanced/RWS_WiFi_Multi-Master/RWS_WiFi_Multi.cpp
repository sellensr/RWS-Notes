// You can create a .cpp file in the IDE. It doesn't need to have C++ code.
// It will be compiled separately, so it needs to include the .h file.
#include "RWS_WiFi_Multi.h"

char **multi_ssid = 0;
char **multi_user = 0;
char **multi_pass = 0;
byte *multi_allow = 0;
unsigned multi_NSSID = 0;
unsigned multi_MAX_SSID = 0;
String ssidNew = "", userNew = "", passNew = "";
char ssidBase[40] = "Signature";
char passBase[40] = "anemone2";
// Call once to initialize hardware settings and the multiple network credentials.
// Uses pointers, so changes in the list will print through from the main().
int WiFi_Multi_Init(char **s, char **u, char **p, byte *a, unsigned m){
#if !defined(ARDUINO_SAMD_MKR1000) && defined(ARDUINO_ARCH_SAMD) // Feather M0 WINC1500
  WiFi.setPins(8,7,4,2);  // Set the pins to match a Feather M0 WINC1500
#endif
  // arguments are assumed to be valid -- some testing could be a good idea!
  multi_ssid = s;
  multi_user = u;
  multi_pass = p;
  multi_allow = a;
  multi_MAX_SSID = m;
  //Scan the list until you find a null string and keep count of how many.
  for(multi_NSSID = 0;multi_ssid[multi_NSSID][0];multi_NSSID++);
  return multi_NSSID;
}

// need a function to add to the lists

// Try the ssids remembered by the microcontroller
// A WiFi.disconnect() will prevent ESP8266 from restarting a previous connection,
// but will not prevent ESP32. The ESP32 retains multiple previous networks 
// somewhere in flash. I succeeded in eliminating them by using esptool.py under 
// Anaconda to erase the complete flash space. Go to the root environment and open 
// a terminal, then type:
//    pip install esptool
//    python esptool.py --port /dev/cu.SLAB_USBtoUART erase_flash
// Get the port from the Arduino IDE so you know where the ESP is attached. 
// Be sure to close the Arduino IDE first so that the port is available
int WiFi_Multi_Memory(unsigned t){
  // This code gets a connection with ESP32 if it has previously made a connection,
  // but not with M0 WinC1500 Feather. Also works with ESP8266, but I had to add the
  // yield() calls to avoid a wdt reset. Times out if no connection after t seconds.
  WiFi.begin();
  unsigned long ts = millis();
  while (WiFi.status() != WL_CONNECTED && (millis() - ts)/1000 < t) yield();
  return WiFi.status();
}

// Try the first available ssid from the list
int WiFi_Multi_List(unsigned t){
  unsigned long ts = millis();
  int ssidIndex = findNextWiFi(0);
  // If there is a matching network and no connection made yet
  if (ssidIndex >= 0) {     
    WiFi.disconnect();      // shut down any existing connection
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(multi_ssid[ssidIndex]);
    WiFi.begin(multi_ssid[ssidIndex], multi_pass[ssidIndex]);
    // wait until connected or timed out
    while ((millis() - ts)/1000 < t && WiFi.status() != WL_CONNECTED) yield();
  }
  return WiFi.status();
}


// Setup an access point and get user input about which network to sign up to
int WiFi_Multi_AP(char *ssidAP, char *passAP, unsigned t){
  // Some code adapted from https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
  WiFiServer server(80);
  int n = WiFi.scanNetworks();      // the total number of networks out there
  int nPicked = -1;                 // the network selected from the scan list, -1 for none
  bool answered = false;            // received an answer from the user's browser
  bool shownSSIDs = false;          // have already displayed the network ssid list
  unsigned long ts = millis();      // wait until t seconds after this start time
  ssidNew = userNew = passNew = ""; // no new input from user as yet
  IPAddress APIP;
  // Start up an access point with code that depends on the processor
  #if defined(ESP32) || defined(ESP8266)
    WiFi.softAP(ssidAP,passAP);
    APIP = WiFi.softAPIP();
  #elif defined(WINC1500)
    WiFi.beginAP(ssidAP);
    delay(10000);    // the WiFi101 AP example waited 10000 ms -- why?
    APIP = WiFi.localIP();
  #endif
  Serial.print("WiFi Access Point IP = "); Serial.print(APIP); Serial.print(" (use in browser)\n");
  Serial.print("AP will appear as SSID: "); Serial.print(ssidAP); Serial.print(", with password: "); Serial.println(passAP);
  Serial.print("You have "); Serial.print(t); Serial.print(" seconds before the AP will time out.\n");

  server.begin();   // Use minimal server functionality to avoid implementation differences 

  // Keep trying until you get credentials or time out
  while(!answered && (millis()-ts)/1000 < t){
    WiFiClient client = server.available();   // Listen for incoming clients
    if (client) {                             // If a new client connects,
      Serial.println("New Client.");          // print a message out in the serial port
      String header = "";                     // the full header of the client request
      String currentLine = "";                // the current line of the client request being accumulated
      String firstLine = "";                  // the first line of the client request, all blank to start
      String payload = "";                    // the payload following the client request, all blank to start
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          char c = client.read();             // read a byte, then
          Serial.write(c);                    // print it out the serial monitor
          header += c;
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              // collect the content from a post request
              unsigned long tos = millis();
              while(millis()-tos < 100){ 
                if(client.available()){
                  payload += (char) client.read();
                  tos = millis();
                }
              }
              Serial.print("\npayload =");
              Serial.println(payload);
              // ignore lines after the first to avoid picking up /SSID in Referer:
              firstLine = header.substring(0,header.indexOf('\r'));
              Serial.print("\nfirstLine =");
              Serial.println(firstLine);
              
              // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
              // and a content-type so the client knows what's coming, then a blank line:
              client.println("HTTP/1.1 200 OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              
              // always display the list in response to the first request or not a selection and not credentials
              if(!shownSSIDs || (firstLine.indexOf("/SSID") == -1 && payload.indexOf("USERNAME") == -1)){    
// Display SSID Selection Page
                client.println("<!DOCTYPE html><html>");
                client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                client.println("<link rel=\"icon\" href=\"data:,\">");
                // CSS to style the on/off buttons 
                // Feel free to change the background-color and font-size attributes to fit your preferences
                client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
                client.println(".button { width:100%; background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
                client.println("text-decoration: none; font-size: 16px; margin: 2px; cursor: pointer;}");
                client.println(".button2 {background-color: #555555;}</style></head>");
                 // Web Page Heading
                client.println("<body><h1>Pick a Network</h1>");
                for (int i = 0; i < n; i++) {
                  client.print("<p><a href=\"/SSID");
                  client.print(i);
                  client.print("/\"><button class=\"button\">");
                  client.print(WiFi.SSID(i));
                  client.print("</button></a></p>");
                }              
                client.println("</body></html>");
                // The HTTP response ends with another blank line
                client.println();
                shownSSIDs = true;
              } else if(firstLine.indexOf("/SSID") != -1){  // header should start with GET /SSID###/
// Display Credentials Request
                  String chunk = firstLine.substring(firstLine.indexOf("/SSID")+5); // part after /SSID
                  chunk = chunk.substring(0,chunk.indexOf("/"));              // part before /
                  chunk.trim();                                               // no whitespace
                  nPicked = chunk.toInt();
                  Serial.println(nPicked);
                  if(nPicked >= 0 && nPicked < n){  // chosen value is in range
                    ssidNew = WiFi.SSID(nPicked);
                    Serial.println(ssidNew);
                    // Display the HTML web page requesting user and pass for the SSIDs chosen
                    client.println("<!DOCTYPE html><html>");
                    client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                    client.println("<link rel=\"icon\" href=\"data:,\">");
                    // Web Page Heading
                    client.print("<body><h1>Enter Credentials for: ");
                    client.print(ssidNew); client.println("</h1>");
                    // adapted from the ESP32 Simple Authentication example
                    String msg = "";
                    client.println("<form action='/login' method='POST'>To log in, please use : admin/admin<br>");
                    client.println("User:<input type='text' name='USERNAME' placeholder='user name'><br>");
                    client.println("Password:<input type='password' name='PASSWORD' placeholder='password'><br>");
                    client.println("<input type='submit' name='SUBMIT' value='Submit'></form>" + msg + "<br>");
                    client.println("</body></html>");
                    // The HTTP response ends with another blank line
                    client.println();
                  }
                } else if(payload.indexOf("USERNAME") != -1){
// Extract credentials and say thank you
                  String chunk = payload.substring(payload.indexOf("USERNAME")+9); // part after USERNAME=
                  userNew = chunk.substring(0,chunk.indexOf("&"));                 // part before &
                  chunk = payload.substring(payload.indexOf("PASSWORD")+9);        // part after PASSWORD=
                  passNew = chunk.substring(0,chunk.indexOf("&"));                 // part before &                 
                  Serial.print(ssidNew);Serial.print(", ");Serial.print(userNew);Serial.print(", ");Serial.println(passNew);
                  answered = true;      // I got values
                  // Display the HTML web page saying thank you
                  client.println("<!DOCTYPE html><html>");
                  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                  client.println("<link rel=\"icon\" href=\"data:,\">");
                  // Web Page Heading
                  client.print("<body><h1>Thank you! <br>Disconnecting AP Now...</h1>");
                  client.println("</body></html>");
                  // The HTTP response ends with another blank line
                  client.println();
                }
                              
// Break out of the client.connected() while loop because you have served this client
              break;
            } else { // if you got a newline, then clear currentLine
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
        }
      }
      // Close the connection
      client.stop();
      Serial.println("Client disconnected.");
      Serial.println("");
    }
  }
  WiFi.disconnect();
  delay(1000);
    
  // either it got some answered with some credentials or it timed out
  if (ssidNew != "") {     
    WiFi.disconnect();      // shut down any existing connection
    delay(1000);
    Serial.print("Attempting to connect to SSID: "); Serial.print(ssidNew);
    Serial.print(" with "); Serial.print(ssidBase);
    Serial.print(" and "); Serial.println(passBase);
    // seem to get a failure here with WiFi101 that breaks other attempts later as well
    WiFi.begin(ssidNew.c_str(), passNew.c_str());
    // WiFi.begin(ssidBase,passBase);  // this doesn't help
    // wait until connected or timed out (allowing no more than 30 seconds from now)
    unsigned long tos = millis(); unsigned to = t; if(to > 30) to = 30;
    while ((millis() - tos)/1000 < to && WiFi.status() != WL_CONNECTED) yield();
  }
  return WiFi.status();
}


int listWiFiMine(Serial_ *port) {    // Print a list of known networks from my input.
  for (int i = 0; i < multi_NSSID; i++) {
    if(i<10) port->print("  ");
    else if(i<100) port->print(" ");
    port->print(i);
    port->print(": ");
    port->print(multi_ssid[i]);
    for(int j = 0;j < 32-strlen(multi_ssid[i]);j++) port->print(".");
    port->print(" Enabled, User, Pass = ");
    port->print(multi_allow[i]);
    port->print(", ");
    port->print(multi_user[i]);
    port->print(", ");
    port->print(multi_pass[i]);
    port->print("\n");
  }
  return multi_NSSID;
}

int listWiFiScan(Serial_ *port) {  // Print a list of available networks discovered by scanning.
  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; i++) {
    if(i<10) port->print("  ");
    else if(i<100) port->print(" ");
    port->print(i);
    port->print(": ");
    port->print(WiFi.SSID(i));
    String dummy = WiFi.SSID(i); // to get around WiFi.SSID(i) returning char * in WiFi101
    for(int j = 0;j < 32-dummy.length();j++) port->print(".");
    port->print(" (");
    port->print(WiFi.RSSI(i));
    port->print("db) Encryption Type: ");
    port->print(WiFi.encryptionType(i));
    switch(WiFi.encryptionType(i)){
      case ENC_TYPE_NONE:
        port->println("       None");
        break;
      case ENC_TYPE_WEP:
        port->println("        WEP");
        break;
      case ENC_TYPE_TKIP:
        port->println("    WPA_PSK");
        break;
      case ENC_TYPE_CCMP:
        port->println("   WPA2_PSK");
        break;
      case ENC_TYPE_AUTO:
        port->println("       AUTO");
        break;
      case ENC_TYPE_ENTERPRISE:
        port->println(" ENTERPRISE");
        break;
      default:
        port->println("    Unknown");
        break;
    }
  }
  return n;
}

int existsWiFi(char* ssid) {  // Check to see if an SSID is visible (T/F)
  int n = WiFi.scanNetworks();
  if (n < 1) return false;     // no networks found
  for (int i = 0; i < n; i++) {
    // WiFi.SSID() can return either a String type or a char * type
    // depending on implementation, so coerce it to String() to compare
    String id = String(WiFi.SSID(i));
    if (id.equals(ssid)) return true;
  }
  return false;
}

int findNextWiFi(int start) { // Return the index of the next available network
  if (start >= multi_NSSID) return -1;  // or -1 if there is none available
  int n = WiFi.scanNetworks();
  if (n < 1) return -1;                 // no networks found
  for (int i = 0; i < n; i++) {         // compare my list against those found
    for (int j = start; j < multi_NSSID; j++) {
      // WiFi.SSID() can return either a String type or a char * type
      // depending on implementation, so coerce it to String() to compare
      String id = String(WiFi.SSID(i));
      if (id.equals(multi_ssid[j])) return j;  // the one to try
    }
  }
  return -1;
}
