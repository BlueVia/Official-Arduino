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
#ifndef _GSM3MOBILENETWORKPROVIDER_
#define _GSM3MOBILENETWORKPROVIDER_

#include <GSM3MobileAccessProvider.h>
#include <inttypes.h>
#include <stddef.h>
#include <IPAddress.h>
		
class GSM3MobileNetworkProvider 
{
	private:
		int HWrestart();
		uint16_t socketsAsServer;
		
		// Modem sockets status
		virtual inline bool getSocketAsServerModemStatus(int s){return false;};
		
	public:
		
		virtual inline int minSocketAsServer(){return 0;};
		virtual inline int maxSocketAsServer(){return 0;};
		virtual int ready()=0;
		
		GSM3MobileNetworkProvider();
		virtual inline GSM3_NetworkStatus_t getStatus(){return ERROR;};
		
		// Return 1 if connected, 0 otherwise
		bool getStatusSocketClient(uint8_t socket);

		virtual inline void closeCommand(int code){};
				
		// Server socket functions
		virtual inline int connectTCPServer(int port, char* localIP, int localIPlength){return 0;};
		virtual inline int getIP(char* LocalIP, int LocalIPlength){return 0;};
				
		// return -1 if no new socket has been occupied
		int getNewOccupiedSocketAsServer();
		
		bool getStatusSocketAsServer(uint8_t socket);
		
		int disconnectTCP(bool client1Server0, int idsocket){return 1;};
		void releaseSocket(int socket){};

};

extern GSM3MobileNetworkProvider* theProvider;

#endif
