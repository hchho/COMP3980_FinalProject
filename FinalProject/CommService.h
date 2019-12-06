#pragma once

#include <Windows.h>
#include "CommController.h"
#include "StateController.h"
#include "Statistics.h"

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
--		DATE:			Dec 05, 2019
--		REVISER:		Albert Liu
--		DESCRIPTION:	Added statisticsFunc to display protocol statstics
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

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	protocolFunc
	--
	-- DATE:		Dec 04, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Michael Yu
	--
	-- PROGRAMMER:	Michael Yu
	--
	-- INTERFACE:	DWORD WINAPI protocolFunc(LPVOID param)
	--					LPVOID param:	the input to pass into the threading function
	--
	-- RETURNS:		DWORD
	--
	-- NOTES:
	-- Call this function initiate the protocol thread
	----------------------------------------------------------------------------------------------------------------------*/
	static DWORD WINAPI protocolFunc(LPVOID param) {
		return ((StateController*)param)->handleProtocolWriteEvents();
	};

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	statisticsFunc
	--
	-- DATE:		Dec 05, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Albert Liu
	--
	-- PROGRAMMER:	Albert Liu
	--
	-- INTERFACE:	DWORD WINAPI statisticsFunc(LPVOID param)
	--					LPVOID param:	the input to pass into the threading function
	--
	-- RETURNS:		DWORD
	--
	-- NOTES:
	-- Call this function display protocol statistics
	----------------------------------------------------------------------------------------------------------------------*/
	static DWORD WINAPI statisticsFunc(LPVOID param) {
		return ((Statistics*)param)->drawStatistics();
	};

};