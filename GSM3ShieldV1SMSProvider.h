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
#ifndef __GSM3_SHIELDV1SMSPROVIDER__
#define __GSM3_SHIELDV1SMSPROVIDER__

#include <GSM3ShieldV1ModemCore.h>
#include <GSM3MobileSMSProvider.h>
#include <GSM3ShieldV1SMSProvider.h>


class GSM3ShieldV1SMSProvider : public GSM3MobileSMSProvider, public GSM3ShieldV1BaseProvider
{		
	public:
		GSM3ShieldV1SMSProvider();
		
		void manageResponse(byte from, byte to);
		
		inline int beginSMS(const char* to);
		inline void writeSMS(char c);
		inline int endSMS();
		int availableSMS(); //Check if SMS available and prepare it to be read.
		int peekSMS(); // Read a byte but do not advance the buffer header
		void flushSMS(); // Delete the SMS from Modem memory and process answer.
		int remoteSMSNumber(char* number, int nlength); //Before reading the SMS, read the phone number.
		int readSMS();   // read one char. Advance circular buffer.
		// Returns 0 if last command is still executing
		// 1 if success
		// >1 if error 
		int ready(){return GSM3ShieldV1BaseProvider::ready();};

	private:	
	
		int idSMS; //Id from current SMS being read.
		bool flagReadingSMS; //To detect first SMS char if not yet reading.
		bool fullBufferSMS; //To detect if the SMS being read needs another buffer. 
		bool twoSMSinBuffer; //To detect if the buffer has more than 1 SMS.
		bool checkSecondBuffer; //Pending to detect if the second buffer has more than 1 SMS.

		//SMS private functions.
		void beginSMSContinue();
		void endSMSContinue ();
		void availableSMSContinue();
		void flushSMSContinue();
		bool parseCMGL_available(bool& rsp);
};
#endif