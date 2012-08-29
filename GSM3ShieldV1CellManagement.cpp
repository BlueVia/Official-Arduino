#include <GSM3ShieldV1CellManagement.h>

GSM3ShieldV1CellManagement::GSM3ShieldV1CellManagement()
{
	
}

String GSM3ShieldV1CellManagement::getLocation()
{
	String modemResponse;
	
	modemResponse = modem.writeModemCommand("AT+QENG=1", 2000);
	modemResponse = modem.writeModemCommand("AT+QENG?", 2000);
	// check response
	char res[modemResponse.length()];
	modemResponse.toCharArray(res, modemResponse.length());
	modemResponse = "";
	char* res_tok = strtok(res, ",");
	res_tok = strtok(NULL, ",");
	res_tok = strtok(NULL, ",");
	modemResponse = String(res_tok);
	res_tok = strtok(NULL, ",");
	modemResponse += "," + String(res_tok);
	res_tok = strtok(NULL, ",");
	modemResponse += "," + String(res_tok);
	res_tok = strtok(NULL, ",");
	modemResponse += "," + String(res_tok);

	return modemResponse;
	
}