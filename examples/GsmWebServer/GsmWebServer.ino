/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using a GSM shield.

 Circuit:
 * GSM shield attached
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 8 Mar 2012
 by Tom Igoe
 */

// libraries
#include <GSM3MobileServerService.h>
#include <GSM3ShieldV1ServerProvider.h>
#include <GSM3MobileClientService.h>
#include <GSM3ShieldV1ClientProvider.h>
#include <GSM3ShieldV1AccessProvider.h>
#include <GSM3ShieldV1DataNetworkProvider.h>

// APN data
#define GPRS_APN       "GPRS_APN" // replace your GPRS APN
#define GPRS_LOGIN     "login"    // replace with your GPRS login
#define GPRS_PASSWORD  "password" // replace with your GPRS password

// initialize the library instance
GSM3MobileClientService client;
GSM3ShieldV1ClientProvider s1client;
GSM3ShieldV1DataNetworkProvider gprs;
GSM3ShieldV1AccessProvider gsmAccess;     // include a 'true' parameter for debug enabled
GSM3MobileServerService server(80); // port 80 (http default)
GSM3ShieldV1ServerProvider s1server;

// timeout
const unsigned long __TIMEOUT__ = 10*1000;

void setup()
{
  // initialize serial communications
  Serial.begin(9600);
  
  // connection state
  boolean notConnected = true;
  
  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while(notConnected)
  {
    if((gsmAccess.begin()==GSM_READY) &
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD)==GPRS_READY))
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  
  Serial.println("Connected to GPRS network");
  
  // start server
  server.begin();
  waitForResponseServer();
  
  //Get IP.
  IPAddress LocalIP = gprs.getIPAddress();
  if (!waitForResponseServer())
  {
      Serial.println("Couldn't get server IP address");
      while(true);
  } 
  else
  {
      Serial.println("Server IP address=");
      Serial.println(LocalIP);
  }
}

void loop() {

  // listen for incoming clients
  GSM3MobileClientService client = server.available();

  if (client)
  {  
    while (client.connected())
    {
      if (client.available())
      {
        Serial.println("available data socket.");
        bool sendResponse = false;
        while(char c=client.read()) {
          if (c == '\n') sendResponse = true;
        }

     // if you've gotten to the end of the line (received a newline
     // character) 
       if (sendResponse)
       {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(analogRead(analogChannel));
            client.println("<br />");       
          }
          client.println("</html>");
        }
      }
      //necessary delay
      delay(100);
    }
    
    // give the web browser time to receive the data
   delay(1);

    // close the connection:
    client.stop();
  }
}

/*
  Wait for response network server
*/
bool waitForResponseServer() {
  unsigned long m=millis();
  while(((millis()-m)< __TIMEOUT__ )&&(server.ready()==0)) {
    // wait for a response from the modem:
    delay(1000);
  } 
  if (server.ready()==1) return true;
  else return false;
}

