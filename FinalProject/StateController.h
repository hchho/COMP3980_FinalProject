
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "States.h"
#include "DisplayService.h"
#include "ControlCodes.h"
#include "Events.h"

class CommController;
class SessionService;
class StateController {
private:
	STATES state;
	int synch;
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

	int verifyInput(char* input);

public:

	StateController() : comm(nullptr), serv(nullptr), sessionService(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sessionService(sess), events(Events()) {};

	void handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events getEvents() { return events; };
	
	void sendCommunicationMessage(DWORD event);

	// Getters
	STATES getState() { return state; };


};
#endif