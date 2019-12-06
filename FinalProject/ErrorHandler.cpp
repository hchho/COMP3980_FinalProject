#include "ErrorHandler.h"

//UNCOMMENT AFTER ADDING LIBRARY
//#include "boost/crc.hpp"
#include <string>
#include <numeric>
#include <iomanip>
#include <sstream>

using namespace std;

int convertCharArrayToInteger(char* header);
int convertBinaryToDecimal(int n);
int hex_to_int(char hexArray[]);
string int_to_hex(int my_integer);


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
int checkSumCalculator(char* content) {

	char contentHeader[1017];
	copy(content + 2, content + 1019, contentHeader);

	const string s{ contentHeader };

	// Standard idiom for calculating a CRC-32 checksum using the boost library

	// UNCOMMENT AFTER ADDING LIBRARY
	//boost::crc_32_type crc_result;
	//crc_result.process_bytes(s.data(), s.length());
	//const int checksum = crc_result.checksum();

	//return checksum;
	return 0;
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
bool checksumMatch(char* content) {

	char header[4];
	char data_word[1017];

	copy(content + 2, content + 1019, data_word);
	copy(content + 1019, content + 2023, header);

	const int convertedHeader = hex_to_int(header);
	const int convertedDataWord = checkSumCalculator(data_word);

	return (convertedHeader == convertedDataWord);
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
-- INTERFACE:	string getHexCRC(int decCRC)
--					int decCRC:		integer that contains the decimal value of the CRC
--
-- RETURNS:		string
--					hexadecimal string of the CRC converted from the decimal value
--
-- NOTES:
-- Call this function to convert the CRC value from integer to hexadecimal.
----------------------------------------------------------------------------------------------------------------------*/
string getHexCRC(int decCRC)
{

	char crcArr[4];

	std::stringstream stream;
	stream << std::hex << decCRC;
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
-- INTERFACE:	string getHexCRC(int decCRC)
--					int decCRC:		integer that contains the decimal value of the CRC
--
-- RETURNS:		string
--					integer string of the CRC converted from hexadecimal value
--
-- NOTES:
-- Call this function to convert the CRC value from integer to hexadecimal.
----------------------------------------------------------------------------------------------------------------------*/
string int_to_hex(int my_integer)
{
	std::stringstream sstream;
	sstream << std::hex << my_integer;
	std::string result = sstream.str();

	return result;
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
-- PROGRAMMER:	Chirag Fernandez
--
-- INTERFACE:	int hex_to_int(char hexArray[])
--					char hexArray[] - character array containing the hexadecimal values of the CRC
--
-- RETURNS:		int
--
-- NOTES:
-- Call this function to return an integer value of the hexadecimal CRC value
----------------------------------------------------------------------------------------------------------------------*/
int hex_to_int(char hexArray[])
{

	string hexString(hexArray);

	int x;

	std::stringstream ss;
	ss << std::hex << hexArray;
	ss >> x;

	return x;

}

