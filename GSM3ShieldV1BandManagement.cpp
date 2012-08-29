#include <GSM3ShieldV1BandManagement.h>



GSM3ShieldV1BandManagement::GSM3ShieldV1BandManagement(bool trace): modem(trace)
{
	quectelStrings[UNDEFINED]="";
	quectelStrings[EGSM_MODE]="\"EGSM_MODE\"";
	quectelStrings[DCS_MODE]="\"DCS_MODE\"";
	quectelStrings[PCS_MODE]="\"PCS_MODE\"";
	quectelStrings[EGSM_DCS_MODE]="\"EGSM_DCS_MODE\"";
	quectelStrings[GSM850_PCS_MODE]="\"GSM850_PCS_MODE\"";
	quectelStrings[GSM850_EGSM_DCS_PCS_MODE]="\"GSM850_EGSM_DCS_PCS_MODE\"";	

	bandNames[UNDEFINED]="UNDEFINED";
	bandNames[EGSM_MODE]="E-GSM(900)";
	bandNames[DCS_MODE]="DCS(1800)";
	bandNames[PCS_MODE]="PCS(1900)";
	bandNames[EGSM_DCS_MODE]="E-GSM(900)+DCS(1800) ex: Europe";
	bandNames[GSM850_PCS_MODE]="GSM(850)+PCS(1900) Ex: USA, South Am.";
	bandNames[GSM850_EGSM_DCS_PCS_MODE]="GSM(850)+E-GSM(900)+DCS(1800)+PCS(1900)";	
	
}

GSM3_NetworkStatus_t GSM3ShieldV1BandManagement::begin()
{
	// check modem response
	modem.begin();

	// reset hardware
	modem.restartModem();

	return IDLE;
}

GSM3GSMBand GSM3ShieldV1BandManagement::getBand()
{
  String modemResponse=modem.writeModemCommand("AT+QBAND?", 2000);

	
  for(GSM3GSMBand i=GSM850_EGSM_DCS_PCS_MODE;i>UNDEFINED;i=(GSM3GSMBand)((int)i-1))
  {
    if(modemResponse.indexOf(quectelStrings[i])>=0)
      return i;
  }

  Serial.print("Unrecognized modem answer:");
  Serial.println(modemResponse);
  
  return UNDEFINED;
}

String GSM3ShieldV1BandManagement::getBandName(GSM3GSMBand band)
{
	String result;
	
	result=bandNames[band];
	return result;
}

bool GSM3ShieldV1BandManagement::setBand(GSM3GSMBand band)
{
	String command;
	String modemResponse;
	command="AT+QBAND=";
    command+=quectelStrings[band];
	
	// Quad-band takes an awful lot of time
    modemResponse=modem.writeModemCommand(command, 15000);
	
	if(modemResponse.indexOf("QBAND")>=0)
      return true;
	else
		return false;
}