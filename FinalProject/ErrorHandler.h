#pragma once

#include <iomanip>
#include <numeric>
#include <sstream>
#include <string>
#include <windows.h>

#include "boost/boost/crc.hpp"
#include "error_codes.h"
#include "DisplayService.h"
#include "ControlCodes.h"

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		ErrorHandler.h - A struct that handles error codes and displays an error message
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID handleError(UINT errorCode)
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:		(N/A)
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This is a public struct and should be able to be invoked anywhere in the application. The purpose is
-- to handle any error and use DisplayService to display to the user.
----------------------------------------------------------------------------------------------------------------------*/
struct ErrorHandler {

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	handleErrorCode
	--
	-- DATE:		Sept 28, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	VOID handleError(UINT errorCode)
	--					UINT errorCode:	the error code to be deciphered
	--
	--
	-- RETURNS:		void
	--
	-- NOTES:
	-- Call this function when you want to handle an error code.
	----------------------------------------------------------------------------------------------------------------------*/
	static VOID handleError(UINT errorCode) {
		switch (errorCode) {
		case ERROR_COM_STATE_NULL:
			DisplayService::displayMessageBox("COM state uninitialized");
			break;
		case ERROR_OPEN_PORT:
			DisplayService::displayMessageBox("Error opening COM port");
			break;
		case ERROR_PORT_CONFIG:
			DisplayService::displayMessageBox("Error configuring COM port");
			break;
		case ERROR_PORT_PROP:
			DisplayService::displayMessageBox("Error getting COM properties");
			break;
		case ERROR_RD_THREAD:
			DisplayService::displayMessageBox("Error creating read thread");
		case ERROR_OPEN_FILE:
			DisplayService::displayMessageBox("Error opening file");
		default:
			DisplayService::displayMessageBox("Unknown error detected.");
			break;
		}
	}

	/*
	Verifies Commands code
	Can rework to passs in state and then use a switch to verify if strcmp is taking too long
	or might be easier to just have this pass in 2 params to do input plus expected
	*/
	static boolean verifyControl(char* input, const char* control) {
		return strcmp(input, control);
	}

	static BOOL verifyCommand(const char* input) {
		return false;
	}

	/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	checkSumCalculator
--
-- DATE:		Nov 27, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Chirag Fernandez
--
-- PROGRAMMER:	Chirag Fernandez
--
-- INTERFACE:	int checkSumCalculator(char* content, char* header)
--					char* content:	a char pointer to the the data word extracted from a received frame.
--
--
-- RETURNS:		int
--
-- NOTES:
-- Call this function to calculate the checksum for a given dataword in char array format.
----------------------------------------------------------------------------------------------------------------------*/
	static unsigned int checkSumCalculator(std::string content) {

		// Standard idiom for calculating a CRC-32 checksum using the boost library

		// UNCOMMENT AFTER ADDING LIBRARY
		boost::crc_32_type crc_result;
		crc_result.process_bytes(content.data(), content.length()); // Check 0-1018 bytes
		const unsigned int checksum = crc_result.checksum();

		return checksum;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	checksumMatch
	--
	-- DATE:		Nov 27, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Chirag Fernandez
	--
	-- PROGRAMMER:	Chirag Fernandez
	--
	-- INTERFACE:	bool checksumMatch(int checksum, char* header)
	--					int checksum:	a base ten integer representing a checksum.
	--					char* header:	a char pointer to the check sequence extracted from a received frame.
	--
	-- RETURNS:		void
	--
	-- NOTES:
	-- Call this function when you want to evaluate if two checksums/check sequences are equivalent.
	----------------------------------------------------------------------------------------------------------------------*/
	static bool checksumMatch(std::string content) {

		std::string contentToCheck = content.substr(0, 1019); // This substrings the entire frame minus the checksum and EOT
		const unsigned int checkSumForReceivedFrame = checkSumCalculator(contentToCheck);
		
		std::string receivedChecksum = content.substr(1019, 4); // Minus EOT, last four bytes
		const unsigned int convertedReceivedChecksum = char_to_int(receivedChecksum);

		return checkSumForReceivedFrame == convertedReceivedChecksum;
	}

	static std::string getHexCRC(unsigned int decCRC)
	{
		char crcArr[5];

		std::stringstream stream;
		stream << std::hex << decCRC;
		std::string result(stream.str()); // String form of the hex value

		std::string hexArr[4] = { result.substr(0, 2), result.substr(2, 2),
			result.substr(4, 2), result.substr(6, 2) };

		for (int i = 0; i < 4; i++)
		{
			crcArr[i] = hex_to_int(hexArr[i]);
		}
		crcArr[4] = '\0';

		return crcArr;
	}

	/*
	This gets four bytes from the datagram and converts it to the int
	*/
	static unsigned int char_to_int(std::string charArr) {
		// Get each char and translate them to a corresponding hex value
		// Parse combine the string hex into one large string
		// Convert string hex into an unsigned int
		// return value

		std::string hexString;
		std::stringstream stream;
		for (int i = 0; i < 4; i++) {
			stream << std::hex << (unsigned int)(unsigned char)charArr[i];
		}
		hexString += stream.str();

		return hex_to_int(hexString);
	}

	static unsigned int hex_to_int(std::string hexString)
	{
		unsigned int x;

		std::stringstream ss;
		ss << std::hex << hexString;
		ss >> x;

		return x;
	}

};