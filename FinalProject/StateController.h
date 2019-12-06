
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
#include <random>

class CommController;
class SessionService;

class StateController {

private:
	STATES state;
	int syncBit = 0;
	int errorCount = 0;
	int resentCounter = 0;
	int reqCounter = 0;
	char* inputBuffer{ 0 };
	boolean output = false;;
	boolean releaseTX = false;
	std::uniform_int_distribution<int> distribution{ 0, 1500 };

	Events* events;
	SessionService* sess;
	CommController* comm;
	DisplayService* serv;
	StateControllerHelper* sHelper;

	// Handles data frame when in data read state and 
	// 
	void setState(STATES state) { this->state = state; };

	void handleControlCode(char* code);
	void parseDataFrame(char* frame);
	void sendCommunicationMessageToCommController(DWORD event);
	void sendFrameToCommController(std::string data, int syncBit);
	int verifyInput(char* input);

public:

	StateController() : comm(nullptr), serv(nullptr), sess(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess) : comm(comm), serv(serv), sess(sess), events(new Events()) {
		state = IDLE;
	};
	std::queue<std::string> outputBuffer;

	DWORD handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events* getEvents() { return events; };

	STATES getState() { return state; };

};
#endif