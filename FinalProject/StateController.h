
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "States.h"
#include "CommController.h"
#include "DisplayService.h"
#include "Events.h"
#include <queue>
#include "SessionService.h"

// Avoid circular dependency
class SessionService;

class StateController {
private:
	STATES state;
	int errorCount = 0;
	int resentCounter = 0;
	char* inputBuffer{ 0 };
	std::queue<char*> outputBuffer;
	//char** outputBuffer;
	boolean output = false;;
	boolean releaseTX = false;
	Events events;

	SessionService* sess;
	CommController* comm;
	DisplayService* serv;

	// Handles data frame when in data read state and 
	void parseDataFrame(char* frame);
	// 
	void handleControlCode(char* code);
	void setState(STATES state) { this->state = state; };
	void sendCommunicationMessage(DWORD event);
public:

	StateController() : comm(nullptr), serv(nullptr), sess(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sess(sess) {};

	void handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events getEvents() { return events; };
	void sendFrame(char* frame);
};
#endif