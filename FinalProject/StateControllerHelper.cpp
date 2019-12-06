#include "StateControllerHelper.h"
#include "ControlCodes.h"
#include "ErrorHandler.h"
#include "boost/boost/crc.hpp"
#include <numeric>
#include <iomanip>
#include <sstream>
#include <ios>
#include <string>

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		StateControllerHelper.cpp - A controller class that handles any external functionality that the
--						StateController requires. Main functionalities consist of packaging and unpackaging the frames
--						that are sent/received.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--
--					std::string buildCRCString(int crc_value);
--					std::string getFrameContent(char* frame);
--					std::string buildFrame(std::string data);
--					void appendDataWithNullChars(std::string& data);
--
-- DATE:			Dec 03, 2019
--
-- REVISIONS:
--					N/A
--
-- DESIGNER:		Chirag Fernandez, Michael Yu
--
-- PROGRAMMER:		Chirag Fernandez, Michael Yu
--
-- NOTES:
-- The controller class manages the functional tasks pertaining to the construction and deconstruction of the frame that
-- is required by the the StateController. CRC generation is handled by this class.
----------------------------------------------------------------------------------------------------------------------*/

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
	// --------------- COPY DATA FROM POSITIONS 2 - 1018 ------------------
	for (auto i = 0; i < data.size(); ++i) {
		frame.push_back(data.at(i));
	}

	appendDataWithNullChars(frame);

	unsigned int crc = ErrorHandler::checkSumCalculator(frame);

	std::string crc_s{ ErrorHandler::getHexCRC(crc) };
	
	frame += crc_s;
	
	frame.push_back(eof);
	return frame;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	buildFrame
--
-- DATE:		Dec 03, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Michael Yu
--
-- PROGRAMMER:	Michael Yu
--
-- INTERFACE:	appendDataWithNullChars(std::string& data)
--						string& data - data that is received from the file that will be packaged into a frame
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to manipulate the actual string value that is passed in. This will create a string of 1017 data bytes
-- using the current 1017 bytes in the input buffer. If there is less than 1017 in the input buffer, this function will
-- pad the remaining bytes of the data with _NUL characters.
----------------------------------------------------------------------------------------------------------------------*/
void StateControllerHelper::appendDataWithNullChars(std::string& data) {
	size_t dataSize = data.size();

	if (dataSize < 1020) {
		while (++dataSize < 1020) {
			data.push_back(_NUL);
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	buildCRCString
--
-- DATE:		Dec 02, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Chirag Fernandez
--
-- PROGRAMMER:	Chirag Fernandez
--
-- INTERFACE:	buildCRCString(int crc_value)
--						int crc_value - integer value of the crc that is calculated from the 1017 bytes of data in the 
--											in the current frame
--
-- RETURNS:		std::string
--
-- NOTES:
-- Call this function to combine the current string with the gnerated CRC value from the boost library. Returns a 
-- new string containing the current state of the frame with the CRC value (1022 bytes).
----------------------------------------------------------------------------------------------------------------------*/
std::string StateControllerHelper::buildCRCString(int crc_value)
{
	const int ARR_SIZE = 4;
	char crcArr[ARR_SIZE];

	std::stringstream stream;
	stream << std::hex << crc_value;
	std::string result(stream.str());

	return result;
}
