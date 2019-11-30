#pragma once

#define STRICT
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <iostream>
#include "ErrorHandler.h"
#include "CommController.h"

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

VOID CommController::writeDataToPort(HANDLE writeThreadHandle, char* frame)
{
	OVERLAPPED overlapped;
	if (!WriteFile(writeThreadHandle, frame, 1024, NULL, &overlapped)) {
		MessageBox(NULL, (LPCWSTR)"WriteFile failed.", (LPCWSTR)"Error", MB_OK);
	}
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
	DWORD bytesReceived, endTime, lastError;
	DWORD dwEvent, dwError;
	char inputBuffer[1];
	OVERLAPPED overlapRead;

	// Set overlap structure
	overlapRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	overlapRead.Offset = 0;
	overlapRead.OffsetHigh = 0;
	
	// Sets Comm Mask
	if (!SetCommMask(commHandle, EV_RXCHAR)) {
		MessageBox(NULL, (LPCWSTR)"SetCommMask failed.", (LPCWSTR)"Error", MB_OK);
	}
	while (isComActive) {
		if (WaitCommEvent(commHandle, &dwEvent, 0)) {
			ClearCommError(commHandle, &dwError, &cs);
			if ((dwEvent & EV_RXCHAR) && cs.cbInQue) {
				if (!ReadFile(commHandle, inputBuffer, 1, &bytesReceived, &overlapRead)) {
					bytesReceived = 0;
					if ((lastError = GetLastError()) == ERROR_IO_PENDING &&
						GetOverlappedResult(commHandle, &overlapRead, &bytesReceived, TRUE) &&
						bytesReceived) {
						drawSingleCharToWindow(*inputBuffer);
					}
					else {
						ClearCommError(commHandle, &lastError, &cs);
						lastError = 0;
					}
				}
				else {
					if (cs.cbInQue) {
						if (bytesReceived) {
							drawSingleCharToWindow(*inputBuffer);
						}
					}
				}
			}
			if (overlapRead.hEvent) {
				ResetEvent(overlapRead.hEvent);
			}
		}
	}
	PurgeComm(commHandle, PURGE_RXCLEAR);
	return 0;
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
		(LPDWORD)& commConfig.dwSize
	);
	if (!CommConfigDialog(portName, *displayService->getWindowHandle(), &commConfig)) {
		ErrorHandler::handleError(ERROR_PORT_CONFIG);
		return;
	}
	SetCommState(commHandle, &commConfig.dcb);
}
