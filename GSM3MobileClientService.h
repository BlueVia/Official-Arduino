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
#ifndef _GSM3MOBILECLIENTSERVICE_
#define _GSM3MOBILECLIENTSERVICE_

#include <GSM3MobileNetworkProvider.h>
#include <Client.h>


class GSM3MobileClientService : public Client
{
	private:
		uint8_t mySocket;
		uint8_t flags;
		// Blocks waiting for an answer
		int waitForAnswer();

	public:
		GSM3MobileClientService(bool synch=true);
		GSM3MobileClientService(int  socket, bool synch);
		// Returns 0 if last command is still executing
		// 1 if success
		// >1 if error 

		int ready();
		// we take this function out as IPAddress is complex to bring to
		// version 1.
		inline int connect(IPAddress, uint16_t);

		// Returns 2 if there are no resources
		int connect(const char *host, uint16_t port);
		void beginWrite(bool sync=false);
		size_t write(uint8_t c);
		size_t write(const uint8_t *buf);
		size_t write(const uint8_t*, size_t);
		void endWrite(bool sync=false);
		uint8_t connected();
		operator bool();
		int read(uint8_t *buf, size_t size);
		int read();
		int available();
		int peek();
		void flush();
		void stop();
		inline int getSocket(){return (int)mySocket;};

		
};


#endif