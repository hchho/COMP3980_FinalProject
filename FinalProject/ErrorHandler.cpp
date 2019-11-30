#include "ErrorHandler.h"

//UNCOMMENT AFTER ADDING LIBRARY
#include "boost/crc.hpp"
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
int checkSumCalculator(char * content) {

	char contentHeader[4];
	copy(content + 1019, content + 2023, contentHeader);

	const string s(contentHeader);

	// Standard idiom for calculating a CRC-32 checksum using the boost library

	// UNCOMMENT AFTER ADDING LIBRARY
    boost::crc_32_type crc_result;
    crc_result.process_bytes(s.data(), s.length());
    const int checksum = crc_result.checksum();

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
bool checksumMatch(char * content) {

	char header[4];
	char data_word[1017];

	copy(content + 2, content + 1019, data_word);
	copy(content + 1019, content + 2023, header);

	const int convertedHeader = hex_to_int(header);
	const int convertedDataWord = checkSumCalculator(data_word);

	return (convertedHeader == convertedDataWord);
}

string getHexCRC(int decCRC)
{

	return int_to_hex(decCRC);
	
}


string int_to_hex(int my_integer)
{
	std::stringstream sstream;
	sstream << std::hex << my_integer;
	std::string result = sstream.str();

	return result;
}

int hex_to_int(char hexArray[])
{

	string hexString(hexArray);

	int x;
	
	std::stringstream ss;
	ss << std::hex << hexArray;
	ss >> x;

	return x;
	
}

