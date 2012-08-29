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
#ifndef _GSM3SHIELDV1DATANETWORKPROVIDER_
#define _GSM3SHIELDV1DATANETWORKPROVIDER_

#include <GSM3MobileDataNetworkProvider.h>
#include <GSM3ShieldV1BaseProvider.h>
#include <GSM3ShieldV1ModemCore.h>
#include <IPAddress.h>

class GSM3ShieldV1DataNetworkProvider : public GSM3MobileDataNetworkProvider, public GSM3ShieldV1BaseProvider
{
	private:
		char* user;
		char* passwd;
		void attachGPRSContinue();
		void detachGPRSContinue();
		bool parseQILOCIP_rsp(char* LocalIP, int LocalIPlength, bool& rsp);
		void getIPContinue();
		int inet_aton(const char* aIPAddrString, IPAddress& aResult);
	public:
	
		GSM3_NetworkStatus_t networkAttach(char* networkId, char* user, char* pass)
		{
			return attachGPRS(networkId, user, pass);
		};
		
		GSM3_NetworkStatus_t networkDetach(){ return detachGPRS();};
		
		GSM3_NetworkStatus_t attachGPRS(char* apn, char* user_name, char* password, bool synchronous=true);
		
		GSM3_NetworkStatus_t detachGPRS(bool synchronous=true);
		
		/** Returns 0 if last command is still executing
			@return 1 if success, >1 if error 
		*/
		int ready(){return GSM3ShieldV1BaseProvider::ready();};
		
		inline GSM3_NetworkStatus_t getStatus(){return theGSM3ShieldV1ModemCore.getStatus();};

		int getIP(char* LocalIP, int LocalIPlength);
		
		IPAddress getIPAddress();
		
		void manageResponse(byte from, byte to);


};

#endif;