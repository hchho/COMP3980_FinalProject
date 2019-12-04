#pragma once

#include <windows.h>
#include "modes.h"
#include "DisplayService.h"
#include "Events.h"

class CommController;
class StateController;
/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		SessionService.h -	A class that handles all session level events according to the OSI network
--										architecture.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID createReadThread(void)
--					VOID handlePortConfig(LPCWSTR portName)
--					VOID handleCommandeMode(UINT Message, WPARAM wParam)
--					VOID handleConnectMode(UINT Message, WPARAM wParam)
--					VOID handleProcess(UINT Message, WPARAM wParam);
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 09, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Modified createReadThread to accept function routine and parameter
--		DATE:			Oct 11, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Modified constructor to accept a pointer to DisplayService
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- The service class handles messages from the system. All actions are mapped to the menu items defined in WINMENU
-- resource file.
----------------------------------------------------------------------------------------------------------------------*/
class SessionService {
private:
	CommController* commController;
	DisplayService* dispService;
	StateController* stateController;
	int currentMode;
	VOID createThread(LPTHREAD_START_ROUTINE func, LPVOID param);
	VOID handleCommandMode(UINT Message, WPARAM wParam);
	VOID handleConnectMode(UINT Message, WPARAM wParam);
	DWORD readFile(LPVOID input);
public:
	static DWORD WINAPI readFunc(LPVOID param) {
		return ((SessionService*)param)->readFile(0);
	}
	SessionService() {};
	SessionService(CommController* controller, DisplayService* dispService) : commController(controller), dispService(dispService){
		currentMode = COMMAND_MODE;
	};
	VOID handleProcess(UINT Message, WPARAM wParam);

	VOID setStateController(StateController* stateController) { this->stateController = stateController; };
	// NEED TO INITIALIZE THESE TWO THREADS SOMEWHERE
	VOID writeToFile(const char*);

	HANDLE writeThread;
	//writeThread = CreateThread(NULL, 0, handleWrite, nullptr, 0, &writeThreadId);
	HANDLE readThread;
	//writeThread = CreateThread(NULL, 0, handleInput, input, 0, &readThreadId);

};