#pragma once

#include <windows.h>
#include "error_codes.h"
#include "DisplayService.h"

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		ErrorHandler.h - A struct that handles error codes and displays an error message
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					VOID handleError(UINT errorCode)
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
-- This is a public struct and should be able to be invoked anywhere in the application. The purpose is 
-- to handle any error and use DisplayService to display to the user.
----------------------------------------------------------------------------------------------------------------------*/
struct ErrorHandler {
	
	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	handleErrorCode
	--
	-- DATE:		Sept 28, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	VOID handleError(UINT errorCode)
	--					UINT errorCode:	the error code to be deciphered
	--
	--
	-- RETURNS:		void
	--
	-- NOTES:
	-- Call this function when you want to handle an error code.
	----------------------------------------------------------------------------------------------------------------------*/
	static VOID handleError(UINT errorCode) {
		switch (errorCode) {
		case ERROR_COM_STATE_NULL:
			DisplayService::displayMessageBox("COM state uninitialized");
			break;
		case ERROR_OPEN_PORT:
			DisplayService::displayMessageBox("Error opening COM port");
			break;
		case ERROR_PORT_CONFIG:
			DisplayService::displayMessageBox("Error configuring COM port");
			break;
		case ERROR_PORT_PROP:
			DisplayService::displayMessageBox("Error getting COM properties");
			break;
		case ERROR_RD_THREAD:
			DisplayService::displayMessageBox("Error creating read thread");
		default:
			DisplayService::displayMessageBox("Unknown error detected.");
			break;
		}
	}
};