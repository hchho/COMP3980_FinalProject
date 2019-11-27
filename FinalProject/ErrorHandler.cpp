#include "ErrorHandler.h"
#include "C:\Users\chira\Downloads\boost_1_71_0\boost\crc.hpp"

using namespace std;
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
--
--
-- RETURNS:		int
--
-- NOTES:
-- Call this function to calculate the checksum for a given dataword in char array format.
----------------------------------------------------------------------------------------------------------------------*/
int checkSumCalculator(char * content) {

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

	int convertedHeader = convertCharArrayToInteger(header);

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
int convertCharArrayToInteger(char* header) {

    int decToSum[4];

    for (int i = 0; i < 4; i++) {

        char byteArray[8];

        for (int j = 0; j < 8; j++) {

            byteArray[j] = *header;
            header++;

        }

        string s(byteArray);
        decToSum[i] = boost::lexical_cast<int>(s);

    }

    int total = accumulate(begin(decToSum), end(decToSum), 0, plus<int>());

}