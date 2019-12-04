
#ifndef STATE_CONTROLLER_HELPER_H
#define STATE_CONTROLLER_HELPER_H
#include <string>

class StateControllerHelper {

public:

	//char* buildFrame(char* data);
	std::string buildCRCString(int crc_value);
	std::string getFrameContent(char* frame);
	std::string buildFrame(std::string data);
	void appendDataWithNullChars(std::string& data);
};

#endif