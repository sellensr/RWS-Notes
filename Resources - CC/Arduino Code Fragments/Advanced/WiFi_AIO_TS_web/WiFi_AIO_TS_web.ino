// Example code for creating a WiFi connection with a web server
// and using the connection to save data to AdafruitIO and ThingSpeak
// for Internet of Things (IOT) applications.
//    - allows a list of multiple possible SSID/password combinations to try
//    - requires installation of Adafruit IO Arduino library and ThingSpeak 
//      library, both provide many more examples for additional functionality.
//    - tested with ESP8266 Huzzah and M0 WINC1500 and should work with others
//
// Rick Sellens 2017-11-12
//
// This code started life as:
//
// Adafruit IO Publish Example adafruit_00_publish
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// defines MAX_SSID and NSSID, IO_USERNAME, IO_KEY, wlan_ssid[NSSID], wlan_pass[NSSID]
#include <ssid.h> // should include personal SSID data arrays like below, or insert right here,
                  // but use caution if you include private information in your source code
//#define MAX_SSID 10     // spaces to reserve in arrays, must be >= than NSSID
//int NSSID = 5;          // spaces initialized in arrays, could change
//const char *wlan_ssid[MAX_SSID] = {"KYC", "Signature", "KYC Harbour", "MITS Yard", "Signature5"};
//const char *wlan_pass[MAX_SSID] = {""   , "p2" , ""           , "p4" , "p5"  };
//
//// adafruit.io identity
//#define IO_USERNAME "username"
//#define IO_KEY "9048a.....0b51"
//
//// thingspeak identity
//unsigned long myThingSpeakChannelNumber = 234567;           
//const char * myThingSpeakWriteAPIKey = "CW3......6P";
    
#include "AdafruitIO_WiFi.h"    // Covers a variety of board options automatically.

/*********** ThingSpeak  **************/
#include "ThingSpeak.h"         
// myThingSpeakChannelNumber and myThingSpeakWriteAPIKey declared in ssid.h
WiFiClient clientTS;            // Create TCP to connect to the ThingSpeak server.

/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;

// declare the 'counter' feed, then setup after io exists as more than just a pointer
AdafruitIO_Feed *counter;
AdafruitIO_Feed *serverConfig;
AdafruitIO_Feed *GHTime;

AdafruitIO_WiFi *ioList[MAX_SSID];    // an array of pointers to possible AIO objects
AdafruitIO_WiFi io1(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS); // primary
AdafruitIO_WiFi *io = &io1;          // a pointer to the last connected AIO object

void AIOWiFiSetup() {     // call instead of io.connect() to create a connection through some SSID
  // fill the array with AIO objects from ssid.h
  for(int i = 0;i < NSSID;i++) 
    ioList[i] = new AdafruitIO_WiFi(IO_USERNAME, IO_KEY, wlan_ssid[i], wlan_pass[i]);
  WiFi.disconnect();            // hang up any existing connection
  delay(100);                   // wait for it to take
  int index = 0;                // the index in the array of AIO objects    
  while(index < NSSID){         // go through the list of possibilities
    io = ioList[index];         // this will be the persistant pointer to the connection
    setFeeds();                 // attach the feeds and subs before you try to connect
    io->connect();   // try a connection, then wait to see if it connects and break out
    unsigned long start = millis();
    while (io->status() < AIO_CONNECTED && millis()-start < 10000){delay(500); Serial.print(".");}
    if(io->status() == AIO_CONNECTED ) break;
    else{                       
      Serial.print("could not connect with "); Serial.println(wlan_ssid[index]);
      index++;                  // try the next possibility in the list
      index = index % NSSID;    // rollover back to zero, beginning of list
    }
  }
  Serial.println(""); Serial.print("WiFi connected to "); Serial.print(wlan_ssid[index]); 
  Serial.print(" with IP address "); Serial.println((IPAddress) WiFi.localIP());
}

void setFeeds() {
  // For no apparent reason, it is important that the feeds be set up right
  // before the call to io->connect() and before calling io->status
  counter = io->feed("counter");            // setup the "counter" feed on AIO
  serverConfig = io->feed("serverconfig");  // setup the "serverConfig" feed on AIO
  GHTime = io->feed("ghtime");              // setup the "GHTime" feed on AIO

  // set up a message handler for the GHTime feed.
  // the handleGHTimeMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  GHTime->onMessage(handleGHTimeMessage);
}
// this function is called whenever a 'GHTime' message
// is received from Adafruit IO. it was attached to
// the GHTime feed in the setFeeds() function above.
void handleGHTimeMessage(AdafruitIO_Data *data) {
  Serial.print("GHTime received <- ");
  Serial.println(data->value());
}

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO\n");

  // connect to io.adafruit.com
//  io->connect();   // with io as a pointer, all the calls need to be made as io->func(), not io.func()
//  setFeeds();      // this needs to happen here before the checking for connection, why?
  AIOWiFiSetup();   // doesn't return until there is a valid connection


//  // wait for a connection
//  while(io->status() < AIO_CONNECTED) {
//    Serial.print(".");
//    delay(500);
//  }
//
//  // we are connected
//  Serial.println();
//  Serial.println(io->statusText());


  webServerSetup();                   // setup a web server that will use the same WiFi connection
  unsigned long current = (unsigned long) WiFi.localIP();   // cast the ip to a simple unsigned long
  byte *curr = (byte *) &current;                           // split the unsigned long into bytes
  String s = "Ex 00 Web Server started at: " + String(curr[0]);
  for(int i = 1;i<4;i++) s += "." + String(curr[i]); // String() doesn't know how to format (IPAdress)
  s += "\n";
  Serial.print("Saving to AIO: "); Serial.print(s);
  serverConfig->save(s);

  ThingSpeak.begin(clientTS);
}

void loop() {
  static unsigned long lastSave = 0;

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io->run();

  webServerLoop();    // give the web server regular service too
  
  if(micros() - lastSave > 10000000L){    // don't save every time through the loop
    lastSave = micros();
    
    // save count to the 'counter' feed on Adafruit IO
    Serial.print("sending -> ");
    Serial.println(count);
    counter->save(count);
  
    // save count to my public test feed on ThingSpeak (defined in ssid.h)
    // visible at https://thingspeak.com/channels/363247
    ThingSpeak.setField(1,count);
    ThingSpeak.writeFields(TSchannelTest, TSkeyTest);
  
    // increment the count by 1
    count++;
  }
}
