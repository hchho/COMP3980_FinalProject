#include "ErrorHandler.h"
#include "C:\Users\chira\Downloads\boost_1_71_0\boost\crc.hpp"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	checkSumCalculator
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Chirag Fernandez
--
-- PROGRAMMER:	Chirag Fernandez
--
-- INTERFACE:	int checkSumCalculator(char* content, char* header)
--					char* content:	a char pointer to the the data word extracted from a received frame.
--					char* header:	a char pointer to the check sequence extracted from a received frame.
--
-- RETURNS:		int
--
-- NOTES:
-- Call this function to calculate the checksum for a given dataword in char array format.
----------------------------------------------------------------------------------------------------------------------*/
int checkSumCalculator(char * content, char * header) {

	// Standard idiom for calculating a CRC-32 checksum using the boost library
    boost::crc_32_type crc_result;
    crc_result.process_bytes(content, size_t contentlength);
    int checksum = crc_result.checksum();

	return checksum;

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	checksumMatch
--
-- DATE:		Sept 28, 2019
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
bool checksumMatch(int checksum, char * header) {

	int convertedHeader = hexadecimalToDecimal(header);

	if (convertedHeader == checksum) {
		return true;
	}
	else {
		return false;
	}

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	hexadecimalToDecimal
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Chirag Fernandez
--
-- PROGRAMMER:	Chirag Fernandez
--
-- INTERFACE:	hexadecimalToDecimal(char * hexVal)
--					char* hexVal:	a pointer to a char array containing a hexadecimal number
--					
--
-- RETURNS:		int
--
-- NOTES:
-- Call this function to convert a hexadecimal number to decimal format.
----------------------------------------------------------------------------------------------------------------------*/
int hexadecimalToDecimal(char * hexVal)
{

	char hexValArray[8];

	for (int i = 0; i < 8; i++) {

		hexValArray[i] = hexVal;
		hexVal++;

	}

	int len = strlen(hexValArray);

	// Initializing base value to 1, i.e 16^0 
	int base = 1;

	int dec_val = 0;

	// Extracting characters as digits from last character 
	for (int i = len - 1; i >= 0; i--)
	{
		// if character lies in '0'-'9', converting  
		// it to integral 0-9 by subtracting 48 from 
		// ASCII value. 
		if (hexVal[i] >= '0' && hexVal[i] <= '9')
		{
			dec_val += (hexVal[i] - 48) * base;

			// incrementing base by power 
			base = base * 16;
		}

		// if character lies in 'A'-'F' , converting  
		// it to integral 10 - 15 by subtracting 55  
		// from ASCII value 
		else if (hexVal[i] >= 'A' && hexVal[i] <= 'F')
		{
			dec_val += (hexVal[i] - 55) * base;

			// incrementing base by power 
			base = base * 16;
		}
	}

	return dec_val;
}