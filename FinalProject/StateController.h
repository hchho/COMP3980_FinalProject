
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "States.h"
#include "CommController.h"
#include "DisplayService.h"
#include "SessionService.h"

class StateController {
private:
	STATES state;
	int errorCount;
	char* inputBuffer;
	char** outputBuffer;
	boolean output;
	Events events;

	SessionService* sessionService;
	CommController* comm;
	DisplayService* serv;

	// Handles data frame when in data read state and 
	void parseDataFrame(char* frame);
	// 
	void handleControlCode(char* code);
	void setState(STATES state) { this->state = state; };

public:

	StateController() : comm(nullptr), serv(nullptr), sessionService(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sessionService(sess), events(new Events()) {};

	void handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events getEvents() { return events; };

};
#endif