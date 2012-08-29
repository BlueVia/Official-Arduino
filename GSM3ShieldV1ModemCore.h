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

#ifndef __GSM3_SHIELDV1MODEMCORE__
#define __GSM3_SHIELDV1MODEMCORE__

#include <GSM3SoftSerial.h>
#include <GSM3ShieldV1BaseProvider.h>
#include <GSM3MobileAccessProvider.h>
#include <Print.h>

#define UMPROVIDERS 3

class GSM3ShieldV1ModemCore : public GSM3SoftSerialMgr, public Print
{
	private:
		//Phone number, used when calling, sending SMS and reading calling numbers
		// Also PIN in modem configuration
		// Also APN
		// Also remote server
		char* phoneNumber;	
		
		// Working port. Port used in the ongoing command, while opening a server
		// Also for IP address length
		int port;

		// 0 = ongoing
		// 1 = OK
		// 2 = Error. Incorrect state
		// 3 = Unexpected modem message
		// 4 = OK but not available data. 
		uint8_t commandError;

		// Counts the steps by the command
		uint8_t commandCounter;
		
		// Presently ongoing command
		GSM3_commandType_e ongoingCommand;
		
		// Enable/disable debug
		bool _debug;
		byte _dataInBufferFrom;
		byte _dataInBufferTo;
		
		// This is the modem (known) status
		GSM3_NetworkStatus_t _status;
		
		GSM3ShieldV1BaseProvider* UMProvider[UMPROVIDERS];
		GSM3ShieldV1BaseProvider* activeProvider;
		
		void manageMsgNow(byte from, byte to);
		
		unsigned long milliseconds;

	public:
		GSM3ShieldV1ModemCore();
		GSM3SoftSerial gss;

		
		// Getters and setters of the running commands
		char *getPhoneNumber(){return phoneNumber;};
		void setPhoneNumber(char *n){phoneNumber=n;};

		int getPort(){return port;};
		void setPort(int p){port=p;};

		uint8_t getCommandError(){return commandError;};
		void setCommandError(uint8_t n){commandError=n;};
		
		uint8_t getCommandCounter(){return commandCounter;};
		void setCommandCounter(uint8_t c){commandCounter=c;};

		GSM3_commandType_e getOngoingCommand(){return ongoingCommand;};
		void setOngoingCommand(GSM3_commandType_e c){ongoingCommand=c;};
		void openCommand(GSM3ShieldV1BaseProvider* activeProvider, GSM3_commandType_e c);
		void closeCommand(int code);

		// These functions allow writing to the SoftwareSerial
		// If debug is set, dump to the console
		size_t write(uint8_t c);
		virtual size_t writePGM(PROGMEM prog_char str[], bool CR=true);
		
		void setDebug(bool db){_debug=db;};
						
		bool genericParse_rsp(bool& rsp, char* string=0, char* string2=0);
		void genericCommand_rq(PROGMEM prog_char str[], bool addCR=true);
		void genericCommand_rqc(const char* str, bool addCR=true);		
		
		inline GSM3CircularBuffer& theBuffer(){return gss.cb;};

		inline void setStatus(GSM3_NetworkStatus_t status) { _status = status; };
		inline GSM3_NetworkStatus_t getStatus() { return _status; };
		
		/** Register provider as willing to receive unsolicited messages
			@param provider	Pointer to provider able to receive unsolicited messages
		*/
		void registerUMProvider(GSM3ShieldV1BaseProvider* provider);		
	
		/** unegister provider as willing to receive unsolicited messages
			@param provider	Pointer to provider able to receive unsolicited messages
		*/
		void unRegisterUMProvider(GSM3ShieldV1BaseProvider* provider);		

		
		/** Register a provider as "dialoguing" talking in facto with the modem
			@param provider	Pointer to provider receiving responses
		*/
		void registerActiveProvider(GSM3ShieldV1BaseProvider* provider){activeProvider=provider;};
		
		/** Needed to manage the SoftSerial. Receives the call when received data
			If _debugging, no code is called
		@param from Starting byte to read
		@param to Last byte to read
		*/
		void manageMsg(byte from, byte to);
		
		/** If _debugging, this call is assumed to be made out of interrupts
			Prints incoming info and calls manageMsgNow
		*/
		void manageReceivedData();

		/** Chronometer. Measure milliseconds from last call
			@return milliseconds from las time function was called
		*/		
		unsigned long takeMilliseconds();
		
		void delayInsideInterrupt(unsigned long milliseconds);

};

extern GSM3ShieldV1ModemCore theGSM3ShieldV1ModemCore;

#endif