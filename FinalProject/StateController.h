
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "States.h"
#include "CommController.h"
#include "DisplayService.h"

class StateController {
private:
	STATES state;
	int errorCount;
	char *inputBuffer;
	char **outputBuffer;
	boolean output;

	CommController *comm;
	DisplayService *serv;

	// Handles data frame when in data read state and 
	void parseDataFrame(char *frame);
	// 
	void handleControlCode(char *code);
	void setState(STATES state) { this->state = state; };

public:
	StateController() {};
	StateController(CommController *comm, DisplayService *serv) : comm(comm), serv(serv) {};

	void handleInput(char* input);
	void drawBufferToWindow(const char *buff);



};
#endif