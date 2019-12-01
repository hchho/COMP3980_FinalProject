
#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H


#include "States.h"
#include "DisplayService.h"
#include "ControlCodes.h"
#include "Events.h"
#include "SessionService.h"
#include "StateControllerHelper.h"
#include <queue>
#include <string>

class CommController;
class SessionService;

class StateController {
private:
	STATES state;
	int synch;
	int errorCount = 0;
	int resentCounter = 0;
	char* inputBuffer{ 0 };
	std::queue<std::string> outputBuffer;
	boolean output = false;;
	boolean releaseTX = false;
	Events events;

	SessionService* sess;
	CommController* comm;
	DisplayService* serv;
	StateControllerHelper* sHelper;

	// Handles data frame when in data read state and 
	void parseDataFrame(char* frame);
	// 
	void handleControlCode(char* code);
	void setState(STATES state) { this->state = state; };

	int verifyInput(char* input);
	void sendCommunicationMessageToCommController(DWORD event);
	void sendFrameToCommController(std::string data);

public:

	StateController() : comm(nullptr), serv(nullptr), sess(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sess(sess), events(Events()) {};

	void handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events getEvents() { return events; };

	// Getters
	STATES getState() { return state; };

};
#endif