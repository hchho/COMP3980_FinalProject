
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


class Statistics;
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
	std::uniform_int_distribution<int> distribution{ 1500, 2500 };

	Events* events;
	SessionService* sess;
	CommController* comm;
	DisplayService* serv;
	StateControllerHelper* sHelper;
	Statistics* stats;

	void setState(STATES state) { this->state = state; };
	void sendCommunicationMessageToCommController(DWORD event);
	void sendFrameToCommController(std::string data, int syncBit);
	int verifyInput(char* input);

public:

	StateController() : comm(nullptr), serv(nullptr), sess(nullptr) {};
	StateController(CommController* comm, DisplayService* serv, SessionService* sess, Statistics* stats) : comm(comm), serv(serv), sess(sess), events(new Events()), stats(stats) {
		state = IDLE;
	};
	std::queue<std::string> outputBuffer;


	char calculateMSBofControlCode(int syncBit);
	DWORD handleProtocolWriteEvents();
	void handleInput(char* input);
	void drawBufferToWindow(const char* buff);
	Events* getEvents() { return events; };
	STATES getState() { return state; };

};
#endif