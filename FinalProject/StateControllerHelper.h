#pragma once
#include <string>

class StateControllerHelper {

public:

	char* StateControllerHelper::buildFrame(char* frame, char syn, char stx, char* data, int data_size, char eof);
	std::string buildCRCString(int crc_value);
	std::string getFrameContent(char* frame);
	
};
