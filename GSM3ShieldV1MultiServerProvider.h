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
#ifndef __GSM3_SHIELDV1SERVERPROVIDER__
#define __GSM3_SHIELDV1SERVERPROVIDER__

#include <GSM3MobileServerProvider.h>
#include <GSM3ShieldV1BaseProvider.h>

class GSM3ShieldV1MultiServerProvider : public GSM3MobileServerProvider,  public GSM3ShieldV1BaseProvider
{
	private:
		uint8_t socketsAsServer;		
		uint8_t socketsAccepted;

		void connectTCPServerContinue();
		void getIPContinue();

		bool parseQILOCIP_rsp(char* LocalIP, int LocalIPlength, bool& rsp);
		void releaseSocket(int socket);

	public:
		GSM3ShieldV1MultiServerProvider();
		int minSocketAsServer(){return 0;};
		int maxSocketAsServer(){return 4;};
		
		bool getSocketAsServerModemStatus(int s);
		int getNewOccupiedSocketAsServer();

		int connectTCPServer(int port);
		int getIP(char* LocalIP, int LocalIPlength);
//		int disconnectTCP(bool client1Server0, int id_socket);
		
		// Returns 0 if last command is still executing
		// 1 if success
		// >1 if error 
		int ready(){return GSM3ShieldV1BaseProvider::ready();};
		
		bool getStatusSocketAsServer(uint8_t socket);
		
		void manageResponse(byte from, byte to);
		bool recognizeUnsolicitedEvent(byte oldTail);

	
};

#endif
