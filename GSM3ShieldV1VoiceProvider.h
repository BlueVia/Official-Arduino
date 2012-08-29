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

#ifndef _GSM3SHIELDV1VOICEPROVIDER_
#define _GSM3SHIELDV1VOICEPROVIDER_

#include <GSM3MobileVoiceProvider.h>
#include <GSM3ShieldV1ModemCore.h>
#include <GSM3ShieldV1BaseProvider.h>

class GSM3ShieldV1VoiceProvider : public GSM3MobileVoiceProvider, public GSM3ShieldV1BaseProvider
{
	public:
		GSM3ShieldV1VoiceProvider();
		void manageResponse(byte from, byte to);

		//Call functions.
		/** Launch a voice call
         @param number 		phone number to be called
		 @return If asynchronous, returns 0. If synchronous, 1 if success, other if error
		*/
		int voiceCall(const char* number);
		int answerCall();
		int hangCall();
		int retrieveCallingNumber(char* buffer, int bufsize);
		
		int ready(){return GSM3ShieldV1BaseProvider::ready();};
		bool recognizeUnsolicitedEvent(byte oldTail);
		
		GSM3_voiceCall_st getvoiceCallStatus(){ready(); return _voiceCallstatus;};
		void setvoiceCallStatus(GSM3_voiceCall_st status) { _voiceCallstatus = status; };

		
	private:
		int phonelength;		//Phone number length.
		// The voiceCall status.
		GSM3_voiceCall_st _voiceCallstatus;

		//CALL private functions.
		void voiceCallContinue();
		void answerCallContinue();
		void hangCallContinue();
		void retrieveCallingNumberContinue();
		
		bool parseCLCC(char* number, int nlength);

};

#endif
