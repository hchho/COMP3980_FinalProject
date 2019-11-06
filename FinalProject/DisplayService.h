#pragma once

#include <windows.h>
#include <stdlib.h>
#include "utils.h"

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:			DisplayService.h -	A service class that handles display events from the application.
--
-- PROGRAM:				DumbSerialPortEmulator
--
-- FUNCTIONS:
--						VOID displayMessageBox(const char * content)
--						VOID drawInput(char input)
--						VOID drawStringBuffer(const char* readBuffer, char delimiter = '0')
--						void setMenuItemState(BOOL state)
--
--
-- DATE:				Sept 28, 2019
--
-- REVISIONS:		
--		DATE:			Oct 10, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Added drawStringBuffer method to display a string
--		DATE:			Oct 11, 2019
--		REVISER:		Henry Ho
--		DESCRIPTION:	Added setMenuItemState to toggle menu item state
--
-- DESIGNER:			Henry Ho
--
-- PROGRAMMER:			Henry Ho
--
-- NOTES:
-- This service class can be used to display any visual out put in the application. It should be used for events
-- such as dialogs, message boxes, and drawing.
----------------------------------------------------------------------------------------------------------------------*/
class DisplayService {
private:
	int xCoord = 0;
	int yCoord = 0;
	HWND* windowHandle;
public:
	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	displayMessageBox
	--
	-- DATE:		Sept 28, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	VOID displayMessageBox(const char * content)
	--					const char * content:	the char content to display in the message box
	--
	-- RETURNS:		void
	--
	-- NOTES:
	-- Call this function to display a message box in the application.
	----------------------------------------------------------------------------------------------------------------------*/
	static void displayMessageBox(const char* content) {
		MessageBox(NULL, utils::strToLPCWSTR(content), TEXT(""), MB_OK);
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	displayMessageBox
	--
	-- DATE:		Sept 28, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	VOID displayMessageBox(LPCWSTR content)
	--					LPCWSTR:	the char content to display in the message box
	--
	-- RETURNS:		void
	--
	-- NOTES:
	-- Call this function to display a message box in the application.
	----------------------------------------------------------------------------------------------------------------------*/
	static void displayMessageBox(LPCWSTR content) {
		MessageBox(NULL, content, TEXT(""), MB_OK);
	}
	DisplayService(HWND* hwnd) : windowHandle(hwnd) {
		setMenuItemState(true);
	};
	VOID drawInput(char input);
	HWND* getWindowHandle();
	VOID drawStringBuffer(const char* readBuffer, char delimiter = '0');
	void setMenuItemState(BOOL state);
};