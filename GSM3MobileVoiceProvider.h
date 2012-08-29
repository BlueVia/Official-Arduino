/*
This file is part of the GSM3 communications library for Arduino
-- Multi-transport communications platform
-- Fully asynchronous
-- Includes code for the Arduino-Telefonica GSM/GPRS Shield V1
-- Voice calls
-- SMS
-- TCP/IP connections
-- HTTP basic clients

This library has been developed by Telef�nica Digital - PDI -
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
#ifndef _GSM3MOBILEVOICEPROVIDER_
#define _GSM3MOBILEVOICEPROVIDER_

enum GSM3_voiceCall_st { IDLE_CALL, CALLING, RECEIVINGCALL, TALKING};

class GSM3MobileVoiceProvider
{
	public:
		//Call functions.
		virtual int voiceCall(const char* number)=0;
		virtual int answerCall()=0;
		virtual int hangCall()=0;
		virtual int retrieveCallingNumber(char* buffer, int bufsize)=0;

		virtual GSM3_voiceCall_st getvoiceCallStatus()=0;
		virtual void setvoiceCallStatus(GSM3_voiceCall_st status)=0;


		virtual int ready()=0;
};

extern GSM3MobileVoiceProvider* theGSM3MobileVoiceProvider;

#endif
