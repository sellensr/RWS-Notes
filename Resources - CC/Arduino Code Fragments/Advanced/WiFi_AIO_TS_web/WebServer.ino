// Adapted from code created 13 July 2010 by dlf (Metodo2 srl) modified 31 May 2012 by Tom Igoe

#ifndef DEBUG
#define DEBUG 9
#endif

#ifndef DBS
#define DBS Serial            // Debug port setting for Serial.printing
#endif

#define DBSA DBS.availableForWrite()
#define DBSP if(DBSA && DEBUG)DBS.print(" ");if(DBSA && DEBUG)DBS.print
#define DBSPNSP if(DBSA && DEBUG)DBS.print

#define WAIT_AFTER_WRITE 2 

WiFiServer server(80);     // a web server on port 80 where you would expect it

void webServerSetup() {      // Start all the servers, assumes WiFi Connection already exists
  DBSP("\nin webServerSetup() WiFi.status() ="); DBSP(WiFi.status());  // fails here with no WiFi 
  // start the server
  server.begin();
  DBSP("\nWeb Server started at:"); DBSP((IPAddress) WiFi.localIP()); DBSP("\n");
}

void webServerLoop() {   // Manage the web clients and disconnect them at the end of each transaction
  // listen for incoming clients
  WiFiClient client = server.available();
  unsigned long startLoop = millis();       // track how long you have been in the function
  unsigned page = 0;                        // return the index page by default
  if (client) {                             // there has been a request
    DBSP("new client page:");               // print a message out the serial port
    String currentLine = "";                // make String to hold incoming data from the client
    // loop while the client's connected and not timed out
    while (client.connected() && (millis() - startLoop) < 500) {
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so act and then send a response:
          if (currentLine.length() == 0) {
            unsigned long pageStart = millis();
            switch(page){
              case 0:                       // add other cases for other page numbers
                writePageIndex(client);
                break;
              default:
                writePageIndex(client);
            }
            break;                          // break out of the while loop:
          }
          else {      // if you got a newline, then parse and clear currentLine:
            DBSP("currentLine:"); DBSP(currentLine);
            //extract whatever information you need from the current line before throwing it away
            // Check to see if the client request was "GET /something" for page selection:
            if (currentLine.indexOf("GET /index") >= 0) page = 0;
            if (currentLine.indexOf("GET /nmea") >= 0) page = 2;
            if (currentLine.indexOf("GET /combo") >= 0) page = 4;
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // give the web browser time to receive the data
    delay(WAIT_AFTER_WRITE);

    // close the connection:
    client.stop();
    DBSP("client disconnected");
  }
}

void writePageIndex(WiFiClient client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // the content of the HTTP response follows the header:
  client.println("A web page is here -- you need to add html content");
  
  // The HTTP response ends with another blank line:
  client.println();
 }

