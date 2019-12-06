#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include "DisplayService.h"
#include "WINDOW.h"
#include "idm.h"
#include "modes.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		DisplayService.cpp -	A service class that handles display events from the application.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID displayMessageBox(const char * content)
--					VOID drawInput(char input)
--
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:		(N/A)
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This service class can be used to display any visual out put in the application. It should be used for events
-- such as dialogs, message boxes, and drawing.
----------------------------------------------------------------------------------------------------------------------*/

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
-- INTERFACE:	VOID drawInput(char input)
--					char input:	the input to draw on the screen
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to draw a character in the screen
----------------------------------------------------------------------------------------------------------------------*/
VOID DisplayService::drawInput(char input) {
	HDC deviceContext = GetDC(*windowHandle);// get device context	
	TEXTMETRIC textMetric;
	char str[255];
	GetTextMetrics(deviceContext, &textMetric);
	sprintf(str, "%c", input); // convert char to str
	TextOut(deviceContext, textMetric.tmMaxCharWidth * xCoord, yCoord, (LPCWSTR)str, strlen(str)); // output character

	// Resets cursor
	if ((xCoord + 1) * textMetric.tmMaxCharWidth >= WINDOW_WIDTH) {
		yCoord = yCoord + textMetric.tmHeight + textMetric.tmExternalLeading;
		xCoord = 0;
	}
	else {
		xCoord++; // increment the screen x-coordinate
	}

	ReleaseDC(*windowHandle, deviceContext); // Release device context
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	drawStringBuffer
--
-- DATE:		Oct 11, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Michael Yu
--
-- PROGRAMMER:	Michael Yu
--
-- INTERFACE:	VOID drawStringBuffer(const char* readBuffer, char delimiter)
--					const char* readBuffer:	the buffer to dispaly on the screen
--					char delimiter:			the delimiter to add after the readbuffer
--
-- RETURNS:		VOID
--
-- NOTES:
-- Call this function to display a string on the screen
----------------------------------------------------------------------------------------------------------------------*/
VOID DisplayService::drawStringBuffer(const char* readBuffer, char delimiter) {
	char buffer[1028]{ 0 };
	strcpy(buffer, readBuffer);
	
	xCoord = 0;
	HDC deviceContext = GetDC(*windowHandle);// get device context	
	TEXTMETRIC textMetric;
	GetTextMetrics(deviceContext, &textMetric);
	TextOut(deviceContext, textMetric.tmMaxCharWidth * xCoord, yCoord, utils::strToLPCWSTR(buffer), strlen(buffer)); // output string of characters

	if (delimiter == 'n') {
		// Resets cursor
		yCoord = yCoord + textMetric.tmHeight + textMetric.tmExternalLeading;
	}
	if (delimiter == 'm')
		yCoord = 0;
	xCoord = 0;

	ReleaseDC(*windowHandle, deviceContext); // Release device context
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	getWindowHandle
--
-- DATE:		Sept 30, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Henry Ho
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:	HWND *  getWindowHandle()
--
-- RETURNS:		pointer to window handle
--
-- NOTES:
-- Call this function to return the window handle
----------------------------------------------------------------------------------------------------------------------*/
HWND* DisplayService::getWindowHandle() {
	return windowHandle;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	setMenuItemState
--
-- DATE:		September 30, 2019
--
-- REVISIONS:	
--		DATE:			Oct 11, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Removed check for isComActive. Added boolean value to toggle menu item state
--		DATE:			Oct 12, 2019
--		REVISIER:		Michael Yu
--		DESCRIPTION:	Change check from binary boolean value to int state
--
-- DESIGNER:	Michael Yu
--
-- PROGRAMMER:	Michael Yu
--
-- INTERFACE:	void changeMenuItemState(int state)
--					int state: value representing the current mode of connection as indicated in modes.h
--
-- RETURNS:		void
--
-- NOTES:
-- If the application is in connect mode, call this function to disable all menu item selections under "Connect", as well as
-- enable the 'Disconnect' menu item. If the application is in command mode, call this function to enable all
-- menu item selections under "Connect", and disable the 'Disconnect' menu item.
----------------------------------------------------------------------------------------------------------------------*/
void DisplayService::setMenuItemState(int state) {
	HMENU menu = GetMenu(*windowHandle);
	EnableMenuItem(menu, IDM_Connect_COM1, state == CONNECT_MODE ? MF_DISABLED : MF_ENABLED);
	EnableMenuItem(menu, IDM_COM1, state == CONNECT_MODE ? MF_DISABLED : MF_ENABLED);
	EnableMenuItem(menu, IDM_Disconnect, state == CONNECT_MODE ? MF_ENABLED : MF_DISABLED);
}