#pragma once

#include <Windows.h>
#include "CommController.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		CommService.cpp -	A service class that handles threading functions in the application in
--											the physical layer
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					DWORD WINAPI readFunc(LPVOID param)
--
-- DATE:			Oct 11, 2019
--
-- REVISIONS:		
--		DATE:			Oct 13, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Updated RFID func to use event driven methods
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This service is used to contain all read functions to pass into threads. Note that it is a struct and methods and
-- member variables are public access. Only use this class to store functions in the physical layer
----------------------------------------------------------------------------------------------------------------------*/
struct CommService {
	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	readFunc
	--
	-- DATE:		Sept 28, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	DWORD WINAPI readFunc(LPVOID param)
	--					LPVOID param:	the input to pass into the threading function
	--
	-- RETURNS:		DWORD
	--
	-- NOTES:
	-- Call this function initiate a reading sequence for a COM port
	----------------------------------------------------------------------------------------------------------------------*/
	static DWORD WINAPI readFunc(LPVOID param) {
		return ((CommController*)param)->handleRead(0);
	};

};