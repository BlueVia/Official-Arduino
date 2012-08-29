/*
  GSM Twitter Client with Strings
 
 This sketch connects to Twitter using using an Arduino GSM shield. 
 It parses the XML returned, and looks for <text>this is a tweet</text>
 
 This example uses the String library, which is part of the Arduino core from
 version 0019.  
 
 Circuit:
 * GSM shield attached to pins 10, 11, 12, 13
 
 created 8 Mar 2012
 by Tom Igoe
 
 This code is in the public domain.
 
 */

#include <GSM3.h>

// initialize the library instance:
GprsClient client;

const unsigned long requestInterval = 30*1000;    // delay between requests; 30 seconds

IPAddress server(199,59,149,200);      // api.twitter.com

boolean requested;                     // whether you've made a request since connecting
unsigned long lastAttemptTime = 0;              // last time you connected to the server, in milliseconds

String currentLine = "";               // string to hold the text from server
String tweet = "";                     // string to hold the tweet
boolean readingTweet = false;          // if you're currently reading the tweet
int status = GSM_IDLE_STATUS;

void setup() {
  // reserve space for the strings:
  currentLine.reserve(256);
  tweet.reserve(150);
  // initialize serial:
  Serial.begin(9600);

  status = gsm.begin();
  if ( status != GSM_CONNECTED) { 
    Serial.println("Couldn't get a GSM connection");
    while(true);
  } 
  else {
    gsm.attachGPRS();
    Serial.println("Connected to GPRS network");
    printGsmStatus();
    connectToServer();
  }
}



void loop()
{
  if (client.connected()) {
    if (client.available()) {
      // read incoming bytes:
      char inChar = client.read();

      // add incoming byte to end of line:
      currentLine += inChar; 

      // if you get a newline, clear the line:
      if (inChar == '\n') {
        currentLine = "";
      } 
      // if the current line ends with <text>, it will
      // be followed by the tweet:
      if ( currentLine.endsWith("<text>")) {
        // tweet is beginning. Clear the tweet string:
        readingTweet = true; 
        tweet = "";
      }
      // if you're currently reading the bytes of a tweet,
      // add them to the tweet String:
      if (readingTweet) {
        if (inChar != '<') {
          tweet += inChar;
        } 
        else {
          // if you got a "<" character,
          // you've reached the end of the tweet:
          readingTweet = false;
          Serial.println(tweet);   
          // close the connection to the server:
          client.stop(); 
        }
      }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and two minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
}

void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(server, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to twitter:
    client.println("GET /1/statuses/user_timeline.xml?screen_name=arduinoteam HTTP/1.1");
    client.println("HOST:api.twitter.com");
    client.println();
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}   


void printGsmStatus() {
  Serial.print("SIM card IMEI: ");
  Serial.println(gsm.imei());
  Serial.print("Band: ");
  Serial.println(gsm.getBand());

  Serial.print("Carrier: ");
  Serial.println(gsm.carrier());
  Serial.print("Signal Strength: ");
  Serial.print(gsm.getSignalStrength());
  Serial.println("dBm");
  Serial.print("Phone number: ");
  Serial.println(gsm.phoneNumber());

  // print your GSM shield's IP address:
  IPAddress ip = gsm.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

