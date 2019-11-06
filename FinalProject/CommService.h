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
--					DWORD WINAPI readRFIDFunc(LPVOID lpParameter)
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

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	readRFIDFunc
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	
	--		DATE:			Oct 13, 2019
	--		REVISER:		Michael Yu
	--		DESCRIPTION:	Updated method to use event-driven process.
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	DWORD WINAPI readRFIDFunc(LPVOID lpParameter)
	--					LPVOID lpParameter: the parameter to pass into the reading function
	--
	-- RETURNS:		DWORD
	--
	-- NOTES:
	-- Call this function to initiate the reading sequence for the RFID device
	----------------------------------------------------------------------------------------------------------------------*/
	static DWORD WINAPI readRFIDFunc(LPVOID lpParameter)
	{
		CommController* controller = (CommController*)lpParameter;
		SKYETEK_STATUS st;
		int isReadState = WaitForSingleObject(controller->getDeviceConnectedEvent(), INFINITE);
		// WAIT_OBJECT_0 is the return value for a successful wait event
		if (isReadState == WAIT_OBJECT_0) {
			controller->getDisplayService()->drawStringBuffer("Entering select tag loop...", 'n');
			st = SkyeTek_SelectTags(controller->getRFIDReader(), AUTO_DETECT, CommController::selectTagLoopCallbackFunc, 0, 1, controller);
			if (st != SKYETEK_SUCCESS) {
				controller->getDisplayService()->drawStringBuffer("Select tag loop failed...", 'n');
			}
			controller->getDisplayService()->drawStringBuffer("Select tag loop done", 'n');

			closeReadersAndDevices(controller);
		}
		return 1;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	initializeRFIDDevice
	--
	-- DATE:		Oct 14, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	SkyeTek
	--
	-- PROGRAMMER:	Michael Yu
	--				Henry Ho
	--
	-- INTERFACE:	static DWORD WINAPI  initializeRFIDDevice(PVOID lpParameter)
	--					PVOID lpParameter: pointer to the parameters required by the thread's main start routine
	--
	-- RETURNS:		static DWORD
	--					denotes the success state resulting from initializing an RFID device
	--
	-- NOTES:
	-- Call this function to find the SkyeTek readers attached to the computer. Returns TRUE if a reader is found,
	-- otherwise it returns a FALSE
	----------------------------------------------------------------------------------------------------------------------*/
	static DWORD WINAPI initializeRFIDDevice(LPVOID lpParameter) {
		CommController* controller = (CommController*)lpParameter;
		bool isComActive = controller->getIsComActive();
		int numDevices = controller->getNumDevices();
		LPSKYETEK_DEVICE* devices = controller->getDevices();
		LPSKYETEK_READER* readers = controller->getReaders();
		unsigned int numReaders = controller->getNumReaders();
		const int NUMBER_OF_TRIES = 10;

		controller->getDisplayService()->drawStringBuffer("Discovering reader...", 'n');
		int attempts = 0;
		while (!isComActive)
		{
			numDevices = SkyeTek_DiscoverDevices(&devices);
			if (numDevices == 0)
			{
				continue;
			}
			if (isComActive)
				return 1;

			numReaders = SkyeTek_DiscoverReaders(devices, numDevices, &readers);
			if (numReaders == 0)
			{
				SkyeTek_FreeDevices(devices, numDevices);
				if (attempts == NUMBER_OF_TRIES) {
					controller->getDisplayService()->drawStringBuffer("No reader found. Program will exit.", 'n');
					return 0;
				}
				attempts++;
				continue;
			}
			else {
				controller->setIsComActive(true);
				break;
			}
		}

		controller->setRFIDReader(readers[0]);
		controller->getDisplayService()->drawStringBuffer("Found reader.", 'n');

		if (!SetEvent(controller->getDeviceConnectedEvent())) {
			controller->getDisplayService()->drawStringBuffer("Failed to set event ", 'n');
		}
		return 1;
	}


	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	closeReadersAndDevices
	--
	-- DATE:		Oct 13, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Michael Yu
	--
	-- PROGRAMMER:	Michael Yu
	--
	-- INTERFACE:	 closeReadersAndDevices(CommController* controller) 
	--					CommController* controller: CommController containing the open readers and devices
	--
	-- RETURNS:		unsigned int
	--					1 if successfully closed
	-- NOTES:
	-- Call this function to close the readers and devices detected from the SkyeTek_DiscoverDevices & SkyeTek_DiscoverReaders
	-- API calls.
	----------------------------------------------------------------------------------------------------------------------*/
	static unsigned int closeReadersAndDevices(CommController* controller) {
		controller->drawBufferToWindow("Exiting select loop callback.", 'n');
		SkyeTek_FreeReaders(controller->getReaders(), controller->getNumReaders());
		controller->drawBufferToWindow("Reader freed.", 'n');
		SkyeTek_FreeDevices(controller->getDevices(), controller->getNumDevices());
		controller->drawBufferToWindow("Device freed.", 'n');
		return 1;
	};
};