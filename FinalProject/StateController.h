
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include "States.h"
#include "DisplayService.h"
#include "ControlCodes.h"
#include "Events.h"
#include "Events.h"
#include <queue>
#include "SessionService.h"

class CommController;
class SessionService;
class StateController {
private:
	STATES state;
	int synch;
	int errorCount = 0;
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

	int verifyInput(char* input);

public:

	StateController() : comm(nullptr), serv(nullptr), sess(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sess(sess), events(Events()) {};

	void handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events getEvents() { return events; };

	// Getters
	STATES getState() { return state; };


	void sendCommunicationMessage(DWORD event);
	void sendFrame(HANDLE writeThreadHandle, char* frame);

};
#endif