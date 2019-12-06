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
--					static boolean verifyControl(char* input, const char* control)
--					static BOOL verifyCommand(const char* input)
--					VOID handleError(UINT errorCode)
--					int checkSumCalculator(char* content)
--					bool checksumMatch(char* content)
--					std::string int_to_hex(int my_integer)
--					int hex_to_int(char hexArray[])
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:		Dec 04, 2019
-- REVISER:			Henry Ho, Chirag Fernandez
-- DESCRIPTION:		Added CRC conversion and error calculation methods
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This is a public struct and should be able to be invoked anywhere in the application. The purpose is
-- to handle any error and use DisplayService to display to the user. This class has now been updated to handle
-- the responsibilities of error checking for CRC values for 1024B checksums
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

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	verifyControl
	--
	-- DATE:		Nov 27, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Chirag Fernandez
	--
	-- PROGRAMMER:	Chirag Fernandez
	--
	-- INTERFACE:	verifyControl(char* input, const char* control) 
	--					char* input:	a char pointer to input to compare
	--					char* control:	a char pointer to the desired control character to compare with
	--
	--
	-- RETURNS:		bool
	--					true if input and control is equal, else false
	--
	-- NOTES:
	-- Call this function to verify that the input and control characters are equal
	----------------------------------------------------------------------------------------------------------------------*/
	static boolean verifyControl(char* input, const char* control) {
		return strcmp(input, control);
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	verifyCommand
	--
	-- DATE:		Nov 27, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Chirag Fernandez
	--
	-- PROGRAMMER:	Chirag Fernandez
	--
	-- INTERFACE:	verifyCommand(const char* input)
	--					const char* input:	a char pointer to the the input
	--
	--
	-- RETURNS:		bool
	--					false
	--
	-- NOTES:
	-- Call this function to veriy the command value
	----------------------------------------------------------------------------------------------------------------------*/
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
	-- INTERFACE:	unsigned int checkSumCalculator(char* content, char* header)
	--					char* content:	a char pointer to the the data word extracted from a received frame.
	--
	--
	-- RETURNS:		unsigned int
	--					unsigned integer value of the checksum for the SYN, STX, and data bytes of a 1024B frame
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
	-- RETURNS:		bool
	--					returns true if the calculated checksum of the first 1019B matches the checksum in the frame payload
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


	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	getHexCRC
	--
	-- DATE:		Nov 27, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Chirag Fernandez
	--
	-- PROGRAMMER:	Chirag Fernandez, Henry Ho
	--
	-- INTERFACE:	string getHexCRC(unsigned int decCRC)
	--					unsigned int decCRC:		unsigned integer that contains the decimal value of the CRC
	--
	-- RETURNS:		string
	--					hexadecimal string of the CRC converted from the decimal value
	--
	-- NOTES:
	-- Call this function to convert the CRC value from integer to hexadecimal.
	----------------------------------------------------------------------------------------------------------------------*/
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

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	hex_to_int
	--
	-- DATE:		Nov 27, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Albert Liu, Henry Ho
	--
	-- INTERFACE:	char_to_int(std::string charArr)
	--					string charArr - character array representing the calculated CRC value
	--
	-- RETURNS:		unsigned int
	--					unsigned integer value of the hexadecimal CRC value
	--
	-- NOTES:
	-- Call this function to convert the array of characters representing the CRC value hexadecimal. The hexadecimal is then
	-- converted to an unsigned integer
	----------------------------------------------------------------------------------------------------------------------*/
	static unsigned int char_to_int(std::string charArr) {
		// Get each char and translate them to a corresponding hex value
		// Parse combine the string hex into one large string
		// Convert string hex into an unsigned int
		// return value

		std::string hexString;
		std::stringstream stream;
		for (int i = 0; i < 4; i++) {
			stream << std::setfill('0') <<std::setw(2)<< std::hex << (unsigned int)(unsigned char)charArr[i];
		}
		hexString += stream.str();

		return hex_to_int(hexString);
	}


	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	hex_to_int
	--
	-- DATE:		Nov 27, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Chirag Fernandez
	--
	-- PROGRAMMER:	Chirag Fernandez, Henry Ho
	--
	-- INTERFACE:	hex_to_int(std::string hexString)
	--					string hexString - hexadecimal string representing the CRC value
	--
	-- RETURNS:		unsigned int
	--					unsigned integer value of the hexadecimal CRC value
	--
	-- NOTES:
	-- Call this function to convert the hexadecimal CRC string value to an unsigned integer
	----------------------------------------------------------------------------------------------------------------------*/
	static unsigned int hex_to_int(std::string hexString)
	{
		unsigned int x;

		std::stringstream ss;
		ss << std::hex << hexString;
		ss >> x;

		return x;
	}

};