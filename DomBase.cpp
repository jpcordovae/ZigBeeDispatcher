#include "DomBase.h"
#include <map>

std::map<std::string,std::string> mapATCommandDescription;

void setMapATCommandDescription(void)
{
    /* Special */
    mapATCommandDescription[C_WR] = "Write";
    mapATCommandDescription[C_RE] = "Restore Defaults";
    mapATCommandDescription[C_FR] = "Software Reset";

    /* Networking & secutiry */
    mapATCommandDescription[C_CH] = "Channel";
    mapATCommandDescription[C_ID] = "PAN ID";
    mapATCommandDescription[C_DH] = "Destination Address High";
    mapATCommandDescription[C_DL] = "Destination Address Low";
    mapATCommandDescription[C_MY] = "16-bit Source Address";
    mapATCommandDescription[C_SH] = "Serial Number High";
    mapATCommandDescription[C_SL] = "Serial Number Low";
    mapATCommandDescription[C_RR] = "XBee Retries";
    mapATCommandDescription[C_RN] = "Random Delay Slots";
    mapATCommandDescription[C_MM] = "MAC Mode";
    mapATCommandDescription[C_NI] = "Node Identifier";
    mapATCommandDescription[C_ND] = "Node Discover";
    mapATCommandDescription[C_NT] = "Node Discover Time";
    mapATCommandDescription[C_DN] = "Destination Node";
    mapATCommandDescription[C_CE] = "Coordinator Enable";
    mapATCommandDescription[C_SC] = "Scan Channels";
    mapATCommandDescription[C_SD] = "Scan Duration";
    mapATCommandDescription[C_A1] = "End Device Association";
    mapATCommandDescription[C_A2] = "Coordinator Association";
    mapATCommandDescription[C_AI] = "Association Indication";
    mapATCommandDescription[C_DA] = "Force Disassociation";
    mapATCommandDescription[C_FP] = "Force Poll";
    mapATCommandDescription[C_AS] = "Active Scan";
    mapATCommandDescription[C_ED] = "Energy Scan";
    mapATCommandDescription[C_EE] = "AES Encryption Enable";
    mapATCommandDescription[C_KY] = "AES Encryption Key";

    /* RF Interfacing */
    mapATCommandDescription[C_PL] = "Power Level";
    mapATCommandDescription[C_CA] = "CCA Threshold";
    mapATCommandDescription[C_SM] = "Sleep";
    mapATCommandDescription[C_ST] = "Time Before Sleep";
    mapATCommandDescription[C_SP] = "Cyclic Sleep Period";
    mapATCommandDescription[C_DP] = "Dissasociated Cyclic Sleep Period";

    /* Serial Interfacing */
    mapATCommandDescription[C_BD] = "Interface Data Rate";
    mapATCommandDescription[C_RO] = "Paquetization Timeout";
    mapATCommandDescription[C_AP] = "API Enable";
    mapATCommandDescription[C_NB] = "Parity";
    mapATCommandDescription[C_PR] = "Pull-Up Resistor";

    /* I/O Settings */
    mapATCommandDescription[C_D8] = "DI8 Conf.";
    mapATCommandDescription[C_D7] = "DIO7 Conf.";
    mapATCommandDescription[C_D6] = "DIO6 Conf.";
    mapATCommandDescription[C_D5] = "DIO5 Conf.";
    mapATCommandDescription[C_D4] = "DIO4 Conf.";
    mapATCommandDescription[C_D3] = "DIO3 Conf.";
    mapATCommandDescription[C_D2] = "DIO2 Conf.";
    mapATCommandDescription[C_D1] = "DIO1 Conf.";
    mapATCommandDescription[C_D0] = "DIO0 Conf.";
    mapATCommandDescription[C_IU] = "I/O Output Enable";
    mapATCommandDescription[C_IT] = "Samples Before TX";
    mapATCommandDescription[C_IS] = "Force Sample";
    mapATCommandDescription[C_IO] = "Digital Output Level";
    mapATCommandDescription[C_IC] = "DIO Change Detect";
    mapATCommandDescription[C_IR] = "Sample Rate";
    mapATCommandDescription[C_AV] = "ADC Voltage Reference";
    mapATCommandDescription[C_IA] = "I/O Input Address";
    mapATCommandDescription[C_T0] = "D0 Output timeout";
    mapATCommandDescription[C_T1] = "D1 Output timeout";
    mapATCommandDescription[C_T2] = "D2 Output timeout";
    mapATCommandDescription[C_T3] = "D3 Output timeout";
    mapATCommandDescription[C_T4] = "D4 Output timeout";
    mapATCommandDescription[C_T5] = "D5 Output timeout";
    mapATCommandDescription[C_T6] = "D6 Output timeout";
    mapATCommandDescription[C_T7] = "D7 Output timeout";
    mapATCommandDescription[C_P0] = "PWM0 Conf.";
    mapATCommandDescription[C_P1] = "PWM1 Conf.";
    mapATCommandDescription[C_M0] = "PWM0 Output Level";
    mapATCommandDescription[C_M1] = "PWM1 Output Level";
    mapATCommandDescription[C_PT] = "PWM Output Timeout";
    mapATCommandDescription[C_RP] = "RSSI PWM Timer";

    /* Diagnostic */
    mapATCommandDescription[C_VR] = "Firmware Version";
    mapATCommandDescription[C_VL] = "Firmware Version - Verbose";
    mapATCommandDescription[C_HV] = "Hardware Version";
    mapATCommandDescription[C_DB] = "Received Signal Strength";
    mapATCommandDescription[C_EC] = "CCA Failure";
    mapATCommandDescription[C_EA] = "ACK Failures";
    mapATCommandDescription[C_ED] = "Energy Scan";

    /* AT Command Options */
    mapATCommandDescription[C_CT] = "Command Mode Timeout";
    mapATCommandDescription[C_CN] = "Exit Command Mode";
    mapATCommandDescription[C_AC] = "Apply Changes";
    mapATCommandDescription[C_GT] = "Guard Times";
    mapATCommandDescription[C_CC] = "Command Sequence Character";

}


uint8_t Char2Hex(char *_c)
{
        uint8_t ucTmp[2];
	int i=0;
	for(i=0; i<2; i++)
	{
		switch( *(_c+i) )
		{
		case '0':	ucTmp[i]=0;
			break;
		case '1':	ucTmp[i]=1;
			break;
		case '2':	ucTmp[i]=2;
			break;
		case '3':	ucTmp[i]=3;
			break;
		case '4':	ucTmp[i]=4;
			break;
		case '5':	ucTmp[i]=5;
			break;
		case '6':	ucTmp[i]=6;
			break;
		case '7':	ucTmp[i]=7;
			break;
		case '8':	ucTmp[i]=8;
			break;
		case '9':	ucTmp[i]=9;
			break;
		case 'A':
		case 'a':
			ucTmp[i]=10;
			break;
		case 'B':
		case 'b':
			ucTmp[i]=11;
			break;
		case 'C':
		case 'c':
			ucTmp[i]=12;
			break;
		case 'D':
		case 'd':
			ucTmp[i]=13;
			break;
		case 'E':
		case 'e':
			ucTmp[i]=14;
			break;
		case 'F':
		case 'f':
			ucTmp[i]=15;
			break;
		default:
			printf("Str2Hex: Caracter no valido en conversion. \n");
                        //TODO: throw this !!
			return -1; //TODO: check this error at up level
		}
	}
        return (ucTmp[0]<<4) + ucTmp[2];
}

