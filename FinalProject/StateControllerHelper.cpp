#include "StateControllerHelper.h"
#include <string>
#include <numeric>
#include <iomanip>
#include <sstream>
#include <ios>

using namespace std;

char* StateControllerHelper::buildFrame(char* frame, char syn, char stx, char * data, int data_size, char eof)
{


	// --------------- IMPLEMENT A CHECK FOR SYNC STATE ------------------
	frame[0] = syn;
	// --------------- IMPLEMENT A CHECK FOR SYNC STATE ------------------
	
	frame[1] = stx;
	strncpy(frame + 2, data, data_size);

	//int crc = calculatecrc(data)
	int crc = 54;

	std::string crc_s = buildCRCString(crc);

	char crc_arr[4];
	strcpy(crc_arr, crc_s.c_str());

	strncpy(frame + 2 + data_size, crc_arr, 4);
	frame[1023] = eof;
	
}

std::string StateControllerHelper::buildCRCString(int crc_value)
{

	char crcArr[4];

	std::stringstream stream;
	stream << std::hex << crc_value;
	std::string result(stream.str());

	string hexArr[4] = { result.substr(0, 2), result.substr(2, 2),
		result.substr(4, 2), result.substr(6, 2) };

	for (int i = 0; i < std::strlen(crcArr); i++)
	{

		stringstream currHex;

		currHex << hex << hexArr[i];
		int tempInt;
		currHex >> tempInt;
		crcArr[i] = tempInt;

	}

	return crcArr;
}

std::string StateControllerHelper::getFrameContent(char* frame)
{
}
