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
#ifndef _GSM3SMSSERVICE_
#define _GSM3SMSSERVICE_

#include <GSM3MobileSMSProvider.h>
#include <Stream.h>

class GSM3SMSService : public Stream 
{
	private:
		uint8_t flags;
		
		// Internal. This function takes responsibility to make
		// synchronous the functions, if needed.
		int waitForAnswerIfNeeded(int returnvalue);
		
	public:
		GSM3SMSService(bool synch=true);
		
		size_t write(uint8_t c);
		int beginSMS(const char* to);
		
		// Returns 0 if last command is still executing
		// 1 if success
		// >1 if error 
		int ready();
		int endSMS();
		
		// Number of bytes in a received SMS
		int available();
		// Give remote sender number.
		int remoteNumber(char* number, int nlength); 
		
		// Read byte. zero if not on message or message is over
		int read();
		
		// Read byte but do not advance
		int peek();
		
		// Finish reading the current packet
		void flush();

};


#endif