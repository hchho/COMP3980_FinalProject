#pragma once

#define STRICT
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include "ErrorHandler.h"
#include "CommController.h"
#include "StateController.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		CommController.cpp -	A controller class that controls all operations in the physical
--												layer in the OSI architecture.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID drawSingleCharToWindow(char input)
--					DWORD handleRead(LPVOID input)
--					VOID handleWrite(WPARAM * input)
--					VOID closePort(void)
--					LPCWSTR getComPortName(void) const
--					VOID handleParam(UINT Msg, WPARAM* wParam)
--					VOID initializeConnection(void)
--					VOID resetCommConfig(void)
--					VOID setComPort(LPCWSTR commPortName)
--					VOID drawBufferToWindow(const char* input)
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 10, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Added drawBufferToWindow to display string to the window
--		DATE:			Oct 11, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Added initializeRFID to start the RFID reader
--		DATE:			Oct 13, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Removed initializeRFID method
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This controller class should be instantiated at the application session level in order to control physical layer
-- functions. This controller class can open ports, close open ports, reset COM port configurations, and handle
-- messages in connection mode.
----------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	closePort
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID closePort(void)
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to close the communication handle.
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::closePort() {
	if (isComActive) {
		CloseHandle(commHandle);
	}
	isComActive = false;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	drawSingleCharToWindow
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID drawSingleCharToWindow(char input)
--					char input:	the input to draw on the screen
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to draw a character in the screen
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::drawSingleCharToWindow(char input) {
	displayService->drawInput(input);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	drawSingleCharToWindow
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID drawBufferToWindow(char input)
--					char input:	the input to draw on the screen
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to draw a char buffer to the screen
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::drawBufferToWindow(const char* input, char delimiter) {
	displayService->drawStringBuffer(input, delimiter);
}

VOID CommController::writeFrameToPort(std::string &frame)
{
	LPCSTR pointerToBufferStart = nullptr;
	pointerToBufferStart = &frame[0];
	// purge output buffer before sending
	int error;
	if (WriteFile(commHandle, pointerToBufferStart, 1024, NULL, &OVERLAPPED())) {
		//DWORD fLen = strlen(frame);
		//if (WriteFile(commHandle, frame, fLen, NULL, &OVERLAPPED())) {
		error = GetLastError();
		DisplayService::displayMessageBox("WriteFile filed for frame");
	}
	PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

VOID CommController::writeControlMessageToPort(const char* controlMessage)
{
	int error;
	DWORD fLen = strlen(controlMessage);
	if (WriteFile(commHandle, controlMessage, fLen, NULL, &OVERLAPPED())) {
		error = GetLastError();
		DisplayService::displayMessageBox("WriteFile filed for code");
	}
	PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}
/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	drawInput
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	BOOL handleParam(UINT Message, WPARAM* wParam)
--					UINT Message:	the message to handle
--					WPARAM* wParam:	the parameter to write if it is a key down event
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to handle messages during connection mode. It returns false to exit current mode, otherwise it
-- returns true.
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::handleParam(WPARAM* wParam) {
	handleWrite(wParam);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleWrite
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID handleWrite(WPARAM* input)
--					WPARAM* input:	the input to write to serial port
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to send a character over the communication port.
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::handleWrite(WPARAM* input) {
	DWORD bytesWritten;
	WriteFile(commHandle, input, 1, &bytesWritten, &OVERLAPPED());
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleRead
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	DWORD handleRead(LPVOID input)
--					LPVOID input:	the input for this function. It is not used in this method.
--
-- RETURNS:		DWORD
--
-- NOTES:
-- This should be called inside a separate thread because of the while-loop inside this function. The ReadFile
-- method uses an overlap structure, which requires event and timeout handling.
--
-- Note that there are missing error handling statements in this method.
----------------------------------------------------------------------------------------------------------------------*/
DWORD CommController::handleRead(LPVOID input) {
	COMSTAT cs;
	DWORD endTime;
	DWORD dwEvent, dwError;


	// Sets Comm Mask
	if (!SetCommMask(commHandle, EV_RXCHAR)) {
		DisplayService::displayMessageBox("SetCommMask failed");
	}
	// Read Loop
	while (isComActive) {
		// Wait for Comm Mask - set to wait for char
		if (WaitCommEvent(commHandle, &dwEvent, 0)) {
			// Clears comm error for readfile also sets cs cbinqueue with how many bytes received by commhandle
			ClearCommError(commHandle, &dwError, &cs);

			//Char event and there is something in queue
			if ((dwEvent && EV_RXCHAR) && cs.cbInQue) {
				// Check state and read specific amount of characters
				//There is a lot of code reuse here we can narrow stuff down
				switch (stateController->getState()) {
				case TX:
					// Expect a REQ or ACK synch bit will be handled in statecontroller 2 bytes
					readHandle(3);
					break;
				case PREP_TX:
					// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
					readHandle(3);
					break;
				case IDLE:
					//Expect a ENQ and only an ENQ Control Code only
					readHandle(3);
					break;
				case RTR:
					//Expect a data frame
					//Or could be an EOT this is the only Staete that should handle either 1028 bytes or 2 byte response
					readHandle(1024);
					break;
				}
			}

			PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
		}
	}

	return 0;
}

void CommController::readHandle(DWORD bytesToReceive) {
	// Control Codes are 2 chars
	char controlBuffer[2];
	char frameBuffer[1024];
	memset(&frameBuffer, 0, sizeof(frameBuffer));
	memset(&controlBuffer, 0, sizeof(controlBuffer));
	DWORD CONTROL_SIZE = 2;
	DWORD lastError;
	DWORD bytesReceived;
	//Expect Control Code

	// Set overlap structure
	OVERLAPPED overlapRead;
	overlapRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	overlapRead.Offset = 0;
	overlapRead.OffsetHigh = 0;

	char* inputBuffer = bytesToReceive > CONTROL_SIZE ? frameBuffer : controlBuffer;
	int isOk = 0;

	// ReadFile
	// Possible issues if we are in a mode expecting control but receive a data frame? PurgeComm() to clear buffer?
	if (ReadFile(commHandle, inputBuffer, bytesToReceive, &bytesReceived, &overlapRead)) {
		//ERROR_IO_PENGING designates if read operation is pending completeion asynchronously
		ClearCommError(this->commHandle, &lastError, NULL);
		int a = GetOverlappedResult(commHandle, &overlapRead, &bytesReceived, TRUE);
		if ((lastError = GetLastError()) == ERROR_SUCCESS &&
			GetOverlappedResult(commHandle, &overlapRead, &bytesReceived, TRUE) &&
			bytesReceived == bytesToReceive) {
			stateController->handleInput(inputBuffer);
		}
		else {
			// Acknowleding there is a error on comm port
			// When do we enter this? last error is not IO PENDING  read operation is pending completeion asynchronously
			//
			ClearCommError(commHandle, &lastError, NULL);
			lastError = 0;
		}
	}
	else {
		// Read file returns false if it fails or is returning asynchronously which is what er're going 
		// Handle issues with actually failing to communitcate here
		// Only for data frames
		GetOverlappedResult(commHandle, &overlapRead, &bytesReceived, TRUE);
		if (bytesReceived == 1024) {
			stateController->handleInput(inputBuffer);
		}
	}
	CloseHandle(overlapRead.hEvent);
	//Clear all characters unread in buffer on handle;
	memset(&frameBuffer, 0, sizeof(frameBuffer));
	PurgeComm(commHandle, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	initializeConnection
--
-- DATE:		Sept 28, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID initializeConnection(LPCWSTR portName)
--					LPCWSTR portName: Name of port to open
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to open the communication port.
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::initializeConnection(LPCWSTR portName) {
	commPortName = portName;
	// Sets up com port/
	if ((commHandle = CreateFile(commPortName, GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL))
		== INVALID_HANDLE_VALUE)
	{
		ErrorHandler::handleError(ERROR_OPEN_PORT);
		return;
	}

	if (!GetCommProperties(commHandle, &commProp)) {
		ErrorHandler::handleError(ERROR_PORT_PROP);
		return;
	};


	SetCommState(commHandle, &commConfig.dcb);

	SetupComm(commHandle, sizeof(OUTPUT_BUFFER), sizeof(OUTPUT_BUFFER));
	LPCWSTR message = TEXT("Connecting to ");
	DisplayService::displayMessageBox((std::wstring(message) + commPortName).c_str());
	isComActive = true;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	setCommConfig
--
-- DATE:		Sept 30, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	VOID setCommConfig(LPCWSTR portName)
--					LPCWSTSR portName: the name of port to configure
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to set the port configurations
----------------------------------------------------------------------------------------------------------------------*/
VOID CommController::setCommConfig(LPCWSTR portName) {
	GetCommConfig(
		commHandle,
		&commConfig,
		(LPDWORD)&commConfig.dwSize
	);
	if (!CommConfigDialog(portName, *displayService->getWindowHandle(), &commConfig)) {
		ErrorHandler::handleError(ERROR_PORT_CONFIG);
		return;
	}
	SetCommState(commHandle, &commConfig.dcb);
}
