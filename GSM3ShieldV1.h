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
#ifndef __GSM3_SHIELDV1__
#define __GSM3_SHIELDV1__

#include <GSM3MobileNetworkProvider.h>
#include <GSM3ShieldV1ModemCore.h>
#include <GSM3ShieldV1BaseProvider.h>
#include <Arduino.h>


class GSM3ShieldV1 : public GSM3MobileNetworkProvider, public GSM3ShieldV1BaseProvider
{
	// General code, for modem management
	private:


		// Delay inside an interrupt
		void delayInsideInterrupt2seg(); 
		
	// Code for SMS Service
	private:


		long commandMillis;
		bool commandSent;
		
		const char* pinConfig;		//PIN.
		char* accessPoint;			//APN.					
		char* userName;				//User.
		char* passw;				//Password.
		const char* remoteID;		//Server.
		
		char* dataSocket;			//Data socket.
		int local_Port;				//Local Port.
		char* local_IP;				//Local IP.
		int local_IP_Length;	//Local IP length.
				
		
		int socketDataSize;  //Size of socket data to be read.
		int socketDataSizeWritten;  //Number of socket data written in buffer not to overflow the buffer
				
		int socketsAccepted;	//Status for remote clients accepted of closed.
			
	public:
		GSM3ShieldV1(bool debug=false);
				

		
		//General functions.
		void manageResponse(byte from, byte to);
		
		// Returns 0 if last command is still executing
		// 1 if success
		// >1 if error 
		int ready(){return GSM3ShieldV1BaseProvider::ready();};
		
		bool genericParse_rsp2(bool& rsp, char* string1, char* string2);
		
		//Configuration functions.

			
		//TCP basic functions.

		
		//URC functions.
		bool recognizeUnsolicitedEvent(byte oldTail);
		bool answerReceived();
		bool socketReceived(int id_socket);
		void update_activeIDsockets (bool active, int ID);
		bool assignIDsocket (int& ID);
		bool closedDataSocket(); //Flag closed current data socket. 
		
		
		//bool writeIncomingCalls(char* bufferForCallerId) Si es cero, ya no espera llamadas
};

#endif