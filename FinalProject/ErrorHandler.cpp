#include "ErrorHandler.hpp"

int checkSumCalculator(char * content, char * header) {

    boost::crc_32_type crc_result;
    crc_result.process_bytes(content, size_t contentlength);
    int checksum = crc_result.checksum();

    return checksum

}

bool checksumMatch(int checksum, char * header) {


}