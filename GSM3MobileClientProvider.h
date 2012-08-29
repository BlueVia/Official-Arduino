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
#ifndef __GSM3_MOBILECLIENTPROVIDER__
#define __GSM3_MOBILECLIENTPROVIDER__

#include <Arduino.h>
#include <IPAddress.h>

class GSM3MobileClientProvider
{
	protected:

		uint8_t sockets;

	
	public:

		GSM3MobileClientProvider(){};
		
		virtual inline int minSocket()=0;
		virtual inline int maxSocket()=0;

		virtual int ready()=0;
		
		// Return 1 if connected, 0 otherwise
		virtual bool getStatusSocketClient(uint8_t socket)=0;

		// Socket management
		virtual int getSocket(int socket=-1)=0;
		virtual void releaseSocket(int socket)=0;


		// Client socket functions
		virtual int connectTCPClient(const char* server, int port, int id_socket)=0;
		virtual int connectTCPClient(IPAddress add, int port, int id_socket)=0;
		virtual void beginWriteSocket(bool client1Server0, int id_socket)=0;
		virtual void writeSocket(const char c)=0;
		virtual void writeSocket(const char* buf)=0;
		virtual void endWriteSocket()=0;
		virtual int availableSocket(bool client, int id_socket)=0;
		virtual int readSocket()=0;
		virtual void flushSocket()=0;
		virtual int peekSocket()=0;
		virtual int disconnectTCP(bool client1Server0, int idsocket)=0;
		



	
};

extern GSM3MobileClientProvider* theGSM3MobileClientProvider;

#endif
