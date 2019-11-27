
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "States.h"
#include "CommController.h"
#include "DisplayService.h"
#include "SessionService.h"
#include "ControlCodes.h"

class StateController {
private:
	STATES state;
	int synch;
	int errorCount;
	char* inputBuffer;
	char** outputBuffer;
	boolean output;

	SessionService* sessionService;
	CommController* comm;
	DisplayService* serv;

	// Handles data frame when in data read state and 
	void parseDataFrame(char* frame);
	// 
	void handleControlCode(char* code);
	void setState(STATES state) { this->state = state; };

	boolean verifyInput(char* input);

public:

	StateController() : comm(nullptr), serv(nullptr), sessionService(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sessionService(sess) {};

	void handleWrite();
	
	//Public method called by commcontroller
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);

	// Getters
	STATES getState() { return state; };


};
#endif