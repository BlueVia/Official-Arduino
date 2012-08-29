/*
  GSM Pachube sensor client
 
 This sketch connects an analog sensor to Pachube (http://www.pachube.com)
 using an Arduino GSM shield.
 

 This example has been updated to use version 2.0 of the Pachube.com API. 
 To make it work, create a feed with a datastream, and give it the ID
 sensor1. Or change the code below to match your feed.
 
 Circuit:
 * Analog sensor attached to analog in 0
 * GSM shield attached to pins 10, 11, 12, 13
 
 created 4 March 2012
 by Tom Igoe
 
 This code is in the public domain.
 
 */


#include <GSM3.h>


#define APIKEY         "YOUR API KEY GOES HERE" // replace your pachube api key here
#define FEEDID         00000 // replace your feed ID
#define USERAGENT      "My Project" // user agent is the project name

char ssid[] = "yourNetwork";          //  your network SSID (name) 
char pass[] = "secretPassword";   // your network password
int status = WL_IDLE_STATUS;

// initialize the library instance:
GprsClient client;

long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
boolean lastConnected = false;      // state of the connection last time through the main loop
const int postingInterval = 10000;  //delay between updates to Pachube.com

void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect to GSM network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  status = gsm.begin();
  if ( status != GSM_CONNECTED) { 
    Serial.println("Couldn't get a GSM connection");
    while(true);
  } 
  else {
    gsm.attachGPRS();
    Serial.println("Connected to GPRS network");
    printGsmStatus();
  }
}


void loop() {
  // read the analog sensor:
  int sensorReading = analogRead(A0);   

  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    sendData(sensorReading);
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

// this method makes a HTTP connection to the server:
void sendData(int thisData) {
  // if there's a successful connection:
  if (client.connect("www.pachube.com", 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.print("PUT /v2/feeds/");
    client.print(FEEDID);
    client.println(".csv HTTP/1.1");
    client.print("Host: api.pachube.com\n");
    client.print("X-PachubeApiKey: ");
    client.println(APIKEY);
    client.print("User-Agent: ");
    client.println(USERAGENT);
    client.print("Content-Length: ");

    // calculate the length of the sensor reading in bytes:
    // 8 bytes for "sensor1," + number of digits of the data:
    int thisLength = 8 + getLength(thisData);
    client.println(thisLength);

    // last pieces of the HTTP PUT request:
    client.print("Content-Type: text/csv\n");
    client.println("Connection: close\n");

    // here's the actual content of the PUT request:
    client.print("sensor1,");
    client.println(thisData);

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }
  lastConnected = client.connected();
}


// This method calculates the number of digits in the
// sensor reading.  Since each digit of the ASCII decimal
// representation is a byte, the number of digits equals
// the number of bytes:

int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten, 
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
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






