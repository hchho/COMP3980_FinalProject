#include "StateControllerHelper.h"
#include "ControlCodes.h"
#include <numeric>
#include <iomanip>
#include <sstream>
#include <ios>
#include <string>

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	buildFrame
--
-- DATE:		Nov 30, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Michael Yu, Chirag Fernandez
--
-- PROGRAMMER:	Michael Yu, Chirag Fernandez
--
-- INTERFACE:
--
-- RETURNS:		string
--
-- NOTES:
-- Call this function to take the string stored in the output buffer and convert it a frame as defined by our protocol.
-- Prepends SYN and STX characters to the data. Appends a 4-byte CRC value and a EOF character to the data.
----------------------------------------------------------------------------------------------------------------------*/
std::string StateControllerHelper::buildFrame(std::string data) {
	std::string frame;
	// --------------- IMPLEMENT A CHECK FOR SYNC STATE ------------------
	frame.push_back(SYN);
	// --------------- IMPLEMENT A CHECK FOR SYNC STATE ------------------
	frame.push_back(STX);
	// --------------- COPY DATA FROM POSITIONS 2 - 1018 -----------------
	for (auto i = 0; i < data.size(); ++i) {
		frame.push_back(data.at(i));
	}

	appendDataWithNullChars(data);

	//TODO MISSING CRC IMPLEMENTATION TO ADD INTO FRAME
	//int crc = calculatecrc(data)
	int crc = 54;

	std::string crc_s{ buildCRCString(crc) };
	char crc_arr[4];
	strcpy(crc_arr, crc_s.c_str());

	// --------------- COPY CRC FROM POSITIONS 1019 - 1022------------------
	for (char c : crc_arr) {
		frame.push_back(c);
	}

	frame.push_back(eof);
	return frame;
}

//char* StateControllerHelper::buildFrame(char* data)
//{
//	char* frame = new char[1024];
//
//	// --------------- IMPLEMENT A CHECK FOR SYNC STATE ------------------
//	frame[0] = SYN;
//	// --------------- IMPLEMENT A CHECK FOR SYNC STATE ------------------
//	frame[1] = STX;
//	// --------------- COPY DATA FROM POSITIONS 2 - 1018 ------------------
//
//	strncpy(frame + 2, data, data_size);
//
//	//int crc = calculatecrc(data)
//	int crc = 54;
//
//	std::string crc_s = buildCRCString(crc);
//
//	char crc_arr[4];
//	strcpy(crc_arr, crc_s.c_str());
//
//	// --------------- COPY CRC FROM POSITIONS 1019 - 1022------------------
//	strncpy(frame + 2 + data_size, crc_arr, 4);
//
//	frame[1023] = eof;
//
//	return frame;
//}

std::string StateControllerHelper::appendDataWithNullChars(std::string& data) {
	size_t dataSize = data.size();

	if (dataSize < 1017) {
		while (++dataSize < 1017) {
			data.push_back(_NUL);
		}
	}
}

std::string StateControllerHelper::buildCRCString(int crc_value)
{

	char crcArr[4];

	std::stringstream stream;
	stream << std::hex << crc_value;
	std::string result(stream.str());

	std::string hexArr[4] = { result.substr(0, 2), result.substr(2, 2),
		result.substr(4, 2), result.substr(6, 2) };

	for (int i = 0; i < std::strlen(crcArr); i++)
	{

		std::stringstream currHex;

		currHex << std::hex << hexArr[i];
		int tempInt;
		currHex >> tempInt;
		crcArr[i] = tempInt;

	}

	return crcArr;
}

//std::string StateControllerHelper::getFrameContent(char* frame)
//{
//}
