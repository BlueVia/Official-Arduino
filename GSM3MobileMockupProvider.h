/*
This file is part of the GSM3 communications library for Arduino
-- Multi-transport communications platform
-- Fully asynchronous
-- Includes code for the Arduino-Telefonica GSM/GPRS Shield V1
-- Voice calls
-- SMS
-- TCP/IP connections
-- HTTP basic clients

This library has been developed by Telefónica Digital - PDI -
- Physical Internet Lab, as part as its collaboration with
Arduino and the Open Hardware Community. 

September-December 2012

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://www.tid.es
*/
#ifndef _GSM3MOBILEMOCKUPPROVIDER_
#define _GSM3MOBILEMOCKUPPROVIDER_

#include <GSM3MobileNetworkProvider.h>
#include <GSM3MobileVoiceProvider.h>
		
class GSM3MobileMockupProvider: public GSM3MobileNetworkProvider
{				
	private:
		// Introducing this status is quite "heavy". But something like this should
		// be added to ShieldV1. Or not. 
		// Note, in ShieldV1 there is no "RECEIVINGSMS" status.
		enum GSM3_modemlinest_e { IDLE, WAITINGANSWER, SENDINGSMS};
		GSM3_modemlinest_e lineStatus;
		char* msgExample;
		int msgIndex;
		
	public:
	
		inline int minSocket(){return 1;};
		inline int maxSocket(){return 8;};

		GSM3MobileMockupProvider();
		inline GSM3_NetworkStatus_t getStatus(){return ERROR;};
		inline GSM3_voiceCall_st getvoiceCallStatus(){return IDLE_CALL;};
		
		int ready();
		inline void closeCommand(int code){};
		
		//Configuration functions.
		int begin(char* pin=0);
		inline int isModemAlive(){return 0;};
		inline bool shutdown(){return false;};
		
		//Call functions.
		inline int voiceCall(const char* number){return 0;};
		inline int answerCall(){return 0;};
		inline int hangCall(){return 0;};
		inline int retrieveCallingNumber(char* buffer, int*& bufsize){return 0;};
		
		// SMS functions
		int beginSMS(const char* number);
		int endSMS();
		int availableSMS();
		int peek();
		void flushSMS();
		int remoteSMSNumber(char* number, int nlength); 
		int readSMS();
		void writeSMS(char c);
		
		// Socket. client.
		int connectTCPClient(const char* server, int port, int id_socket);
		
		// Attention to parameter rewriting in ShieldV1
		void writeSocket(const uint8_t *buf, size_t size, int idsocket);
		
		// ShieldV1 will have two reading mechanisms:
		// Mechanism 1: Call AT+QIRD for size bytes. Put them in the circular buffer,
		// fill buf. Take care to xon/xoff effect, as we may copy just a part of the 
		// incoming bytes.
		int readSocket(uint8_t *buf, size_t size, int idsocket);
		
		// Mechanism 2 in ShieldV1:
		// When called "available()" or "read()" reuse readSocket code to execute 
		// QIRD SYNCHRONOUSLY. Ask the modem for 1500 bytes but do not copy them anywhere, 
		// leave data in the circular buffer. Put buffer head at the start of received data.
		// Peek() will get a character but will not advance the buffer head.
		// Read() will get one character. XON/XOFF will take care of buffer filling
		// If Read() gets to the end of the QIRD response, execute again QIRD SYNCHRONOUSLY 
		// If the user executes flush(), execute read() until there is nothing more to read()
		// (the modem gives no way to empty the socket of incoming data)
		int availableSocket(int idsocket);
		int readSocket(int idsocket, bool advance=true);
		void flushSocket(int idsocket);
		// This is the same in ShieldV1
		int disconnectTCP(int idsocket);
		
		// TCP Server. Attention. Changing the int*&. We'll receive a buffer for the IP
		// If the pointer ins NULL just forget it
		// I think that opening a server does not occupy a socket. Is that true?
		int connectTCPServer(int port, char* localIP, int* localIPlength);
		
		// Modem sockets status. Return TRUE if the modem thinks the socket is occupied.
		// This should be detected through an unrequisited response
		bool getSocketModemStatus(uint8_t s);


};
#endif
