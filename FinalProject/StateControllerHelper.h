
#ifndef STATE_CONTROLLER_HELPER_H
#define STATE_CONTROLLER_HELPER_H
#include <string>

class StateControllerHelper {

public:

	std::string buildCRCString(int crc_value);
	std::string getFrameContent(char* frame);
	std::string buildFrame(std::string data, int syncBit);
	void appendDataWithNullChars(std::string& data);
	void unpackFrame(char* frame);
};

#endif