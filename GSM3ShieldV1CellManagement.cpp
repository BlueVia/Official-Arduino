#include <GSM3ShieldV1CellManagement.h>

GSM3ShieldV1CellManagement::GSM3ShieldV1CellManagement()
{
}

bool GSM3ShieldV1CellManagement::parseQCCID_available(bool& rsp)
{
	char c;
	bool iccidFound = false;
	int i = 0;
	
	while(((c = theGSM3ShieldV1ModemCore.theBuffer().read()) != 0) & (i < 19))
	{
		if((c < 58) & (c > 47))
			iccidFound = true;
		
		if(iccidFound)
		{
			bufferICCID[i] = c;
			i++;
		}
	}
	
	return true;
}			

bool GSM3ShieldV1CellManagement::parseQENG_available(bool& rsp)
{
	char c;
	char location[50] = "";
	int i = 0;
	
	if (!(theGSM3ShieldV1ModemCore.theBuffer().chopUntil("+QENG: ", true)))
		rsp = false;
	else 
		rsp = true;
	
	if (!(theGSM3ShieldV1ModemCore.theBuffer().chopUntil("+QENG:", true)))
		rsp = false;
	else 
		rsp = true;
	
	while(((c = theGSM3ShieldV1ModemCore.theBuffer().read()) != 0) & (i < 50))
	{
		location[i] = c;
		i++;
	}
	
	char* res_tok = strtok(location, ",");
	countryCode = strtok(NULL, ",");
	networkCode = strtok(NULL, ",");
	locationArea = strtok(NULL, ",");
	cellId = strtok(NULL, ",");
	
	return true;
}			

int GSM3ShieldV1CellManagement::getLocation(char *country, char *network, char *area, char *cell)
{
	if((theGSM3ShieldV1ModemCore.getStatus() != GSM_READY) && (theGSM3ShieldV1ModemCore.getStatus() != GPRS_READY))
		return 2;
	
	theGSM3ShieldV1ModemCore.openCommand(this,GETLOCATION);
	getLocationContinue();
	
	unsigned long timeOut = millis();
	while(((millis() - timeOut) < 5000) & (ready() == 0));

	strcpy(country, countryCode);
	strcpy(network, networkCode);
	strcpy(area, locationArea);
	strcpy(cell, cellId);
	
	return theGSM3ShieldV1ModemCore.getCommandError();
}

void GSM3ShieldV1CellManagement::getLocationContinue()
{
	bool resp;
	
	switch (theGSM3ShieldV1ModemCore.getCommandCounter()) {
    case 1:
		theGSM3ShieldV1ModemCore.gss.tunedDelay(3000);
		delay(3000);
		theGSM3ShieldV1ModemCore.setCommandCounter(2);
		theGSM3ShieldV1ModemCore.genericCommand_rq(PSTR("AT+QENG=1"), false);
		theGSM3ShieldV1ModemCore.print("\r");
		break;
	case 2:
		if (theGSM3ShieldV1ModemCore.genericParse_rsp(resp))
		{
			theGSM3ShieldV1ModemCore.gss.tunedDelay(3000);
			delay(3000);
			theGSM3ShieldV1ModemCore.setCommandCounter(3);
			theGSM3ShieldV1ModemCore.genericCommand_rq(PSTR("AT+QENG?"), false);
			theGSM3ShieldV1ModemCore.print("\r");
		}
		else theGSM3ShieldV1ModemCore.closeCommand(1);
		break;
	case 3:
		if (resp)
		{
			parseQENG_available(resp);
			theGSM3ShieldV1ModemCore.closeCommand(3);
		}
		else theGSM3ShieldV1ModemCore.closeCommand(2);
		break;
	}
}

int GSM3ShieldV1CellManagement::getICCID(char *iccid)
{
	if((theGSM3ShieldV1ModemCore.getStatus() != GSM_READY) && (theGSM3ShieldV1ModemCore.getStatus() != GPRS_READY))
		return 2;
	
	theGSM3ShieldV1ModemCore.openCommand(this,GETICCID);
	getICCIDContinue();
	
	unsigned long timeOut = millis();
	while(((millis() - timeOut) < 5000) & (ready() == 0));
	
	strcpy(iccid, bufferICCID);
	
	return theGSM3ShieldV1ModemCore.getCommandError();
}

void GSM3ShieldV1CellManagement::getICCIDContinue()
{
	bool resp;
	
	switch (theGSM3ShieldV1ModemCore.getCommandCounter()) {
    case 1:
		theGSM3ShieldV1ModemCore.setCommandCounter(2);
		theGSM3ShieldV1ModemCore.genericCommand_rq(PSTR("AT+QCCID"), false);
		theGSM3ShieldV1ModemCore.print("\r");
		break;
	case 2:
		if (theGSM3ShieldV1ModemCore.genericParse_rsp(resp))
		{
			parseQCCID_available(resp);
			theGSM3ShieldV1ModemCore.closeCommand(2);
		}
		else theGSM3ShieldV1ModemCore.closeCommand(1);
		break;
	}
}

void GSM3ShieldV1CellManagement::manageResponse(byte from, byte to)
{
	switch(theGSM3ShieldV1ModemCore.getOngoingCommand())
	{
		case NONE:
			theGSM3ShieldV1ModemCore.gss.cb.deleteToTheEnd(from);
			break;
		case GETLOCATION:
			getLocationContinue();
			break;
		case GETICCID:
			getICCIDContinue();
			break;
	}
}