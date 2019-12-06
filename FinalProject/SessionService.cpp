#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include "error_codes.h"
#include "key_press.h"
#include "modes.h"
#include "ErrorHandler.h"
#include "SessionService.h"
#include "CommService.h"
#include "idm.h"
#include "DisplayService.h"
#include "StateController.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		SessionService.cpp -A class that handles all session level events according to the OSI network
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
--				DWORD SessionService::readFile(LPVOID input)
--					VOID SessionService::writeToFile(const char* data)
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 09, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Modified handleCommandMode to allow connect to RFID device option
--		DATE:			Oct 11, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Added option to disconnect and toggle menu item state
--		DATE:			Oct 13, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Updated toggle menu item state method call
--		DATE:			Dec 05, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Added readFile and writeToFile functions for processing file upload and download
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho, Michael Yu
--
-- NOTES:
-- The service class handles messages from the system. All actions are mapped to the menu items defined in WINMENU
-- resource file.
----------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	createReadThread
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 09, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Updated parameters to allow thread function and parameter
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID createReadThread(void)
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to create a thread to enable the application to read inputs from the COM port.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::createThread(LPTHREAD_START_ROUTINE func, LPVOID param) {
	DWORD readThreadId;
	HANDLE readThread;
	readThread = CreateThread(NULL, 0, func, param, 0, &readThreadId);
	if (!readThread) {
		ErrorHandler::handleError(ERROR_RD_THREAD);
	}
	else {
		CloseHandle(readThread);
	}
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleCommandMode
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 09, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Removed extra COM2 port setting and added option to connect to RFID device. Updated
--						createReadThread method call to match header API.
--		DATE:			Oct 14, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Modified Connect RFID mode to use two threads. 
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleCommandMode(UINT Message, WPARAM wParam)
--					UINT Message:	the message dispatched to handle
--					WPARAM wParam:	the parameter attached to the event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function during command mode to handle incoming messages and parameters. If an event is to
-- exit command mode, it configures the current mode to command mode.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::handleCommandMode(UINT Message, WPARAM wParam) {
	switch (Message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_COM1:
			commController->setCommConfig(TEXT("COM1"));
			break;
		case IDM_Connect_COM1:
			commController->initializeConnection(TEXT("COM1"));
			createThread(CommService::readFunc, commController);
			createThread(CommService::protocolFunc, stateController);
			currentMode = CONNECT_MODE;
			dispService->setMenuItemState(currentMode);
			break;
		case IDM_Exit:
			commController->closePort();
			PostQuitMessage(0);
			break;
		case IDM_HELP:
			DisplayService::displayMessageBox("Press <ESC> to disconnect.");
			break;
		default:
			break;
		}
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleConnectMode
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleConnectMode(UINT Message, WPARAM wParam)
--					UINT Message:	the message dispatched to handle
--					WPARAM wParam:	the parameter attached to the event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function during connect mode to handle incoming messages and parameters.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::handleConnectMode(UINT Message, WPARAM wParam) {
	switch (Message) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDM_Exit:
			commController->closePort();
			PostQuitMessage(0);
			currentMode = COMMAND_MODE;
			dispService->setMenuItemState(true);
			break;
		case IDM_Disconnect:
			commController->closePort();
			currentMode = COMMAND_MODE;
			dispService->setMenuItemState(true);
			break;
		case IDM_Upload:
			createThread(SessionService::readFunc, this);
			break;
		case IDM_HELP:
			DisplayService::displayMessageBox("Press <ESC> to disconnect.");
			break;
		default:
			DisplayService::displayMessageBox("In connect mode. Press <ESC> to disconnect.");
			break;
		}
	case WM_CHAR:
		switch (wParam) {
		case ESC_KEY:
			commController->closePort();
			currentMode = COMMAND_MODE;
			dispService->setMenuItemState(true);
			break;
		default:
			//commController->handleParam(&wParam);
			break;
		}
	}

}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleProcess
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleProcess(UINT Message, WPARAM wParam)
--					UINT Message:	the message dispatched to handle
--					WPARAM wParam:	the parameter attached to the event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function in the main window processing function to handle event messages. This function passes
-- the messages to different handlers based on the application's mode.
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::handleProcess(UINT Message, WPARAM wParam) {
	switch (currentMode) {
	case COMMAND_MODE:
		handleCommandMode(Message, wParam);
		break;
	case CONNECT_MODE:
		handleConnectMode(Message, wParam);
		break;
	default:
		break;
	}
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	readFile
--
-- DATE:		Dec 05, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	DWORD readFile(LPVOID input)
--					LPVOID input:	the input for this function. It is not used in this method.
--
-- RETURNS:		DWORD
--
-- NOTES:
-- This should be called inside a separate thread because of the while-loop inside this function. The ReadFile
-- method uses an overlap structure, which requires events and timeout handling. This thread function reads data from
-- the detected file input source at a fixed size, corresponding to the size of the data in our frames (1017B). The 
-- following thread is triggered when the system detects a file input through the upload button of our program.
--
----------------------------------------------------------------------------------------------------------------------*/
DWORD SessionService::readFile(LPVOID input) {
	HANDLE hFile;
	DWORD dwBytesRead;
	CONST UINT MAX_PATH_SIZE = 128;
	CONST UINT MAX_FILE_SIZE = 64000000; // 64 MB file size
	wchar_t filename[MAX_PATH_SIZE];
	OPENFILENAME ofn;

	RtlZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = TEXT("Text Files\0*.txt\0Any File\0*.*\0");
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_FILE_SIZE;
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&ofn)) {
		ErrorHandler::handleError(ERROR_OPEN_FILE);
		return 0;
	}

	hFile = CreateFile(
		filename, 
		GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL
	);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		ErrorHandler::handleError(ERROR_OPEN_FILE);
		return 0;
	}

	char ReadBuffer[1018] = { 0 }; // The buffer size should be defined somewhere
	
	PurgeComm(hFile, PURGE_RXCLEAR);

	int eventIndex = stateController->getState() == RTR ? 5 : 0;
	
	SetEvent(stateController->getEvents()->handles[eventIndex]);

	//It should equal the buffer size - 1 to give room for null character
	while (ReadFile(hFile, ReadBuffer, 1017, &dwBytesRead, NULL)) {
		if (dwBytesRead == 0) {
			break;
		}
		// output buffer is a pointer to char *  if they all point to the same buffer can't really send anything will have to instantiate a new buffer each time 
		
		std::string newBuffer{ ReadBuffer };
	
		stateController->outputBuffer.push(newBuffer);
		
		memset(&ReadBuffer, 0, sizeof(ReadBuffer));
	}
//	ResetEvent(stateController->getEvents()->handles[eventIndex]);

	CloseHandle(hFile);
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	writeToFile
--
-- DATE:		Dec 05, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	DWORD writeToFile(const char* data)
--					const char* data: represents the data received from a frame that will be printed to an arbitrary file
--
-- RETURNS:		VOID
--
-- NOTES:
-- Calling this function will write the characters stored within the buffer to the desginated output file in append mode.
--
----------------------------------------------------------------------------------------------------------------------*/
VOID SessionService::writeToFile(std::string data) {
	std::fstream outputFile;
	outputFile.open("output.txt", std::fstream::app);
	outputFile << data;
	outputFile.close();
}	