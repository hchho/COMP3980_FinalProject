#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <comdef.h>
#include <stdio.h>
#include "key_press.h"
#include "error_codes.h"
#include "ErrorHandler.h"
#include "DisplayService.h"

using namespace std;

/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		CommController.h -	A controller class that controls all operations in the physical
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
--					BOOL getIsComActive(void)
--					void setIsComActive(bool state)
--
-- DATE:			Sept 28, 2019
--
-- REVISIONS:
--		DATE:			Oct 11, 2019
--		REVISER:		Henry Ho & Michael Yu
--		DESCRIPTION:	Added private variables and their respective getters and setters to store SkyeTek
--							API related properties.
--						Added selectTagLoopCallbackFunc function to get information from RFID tags.
--		DATE:			Oct 13, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Moved all non-inline functions to source file and added event handle
--		DATE:			Oct 14, 2019
--		REVISER:		Henry Ho & Michael Yu
--		DESCRIPTION:	Added setIsComActive and added tag map
--		DATE:			Nov 16, 2019
--		REVISER:		Michael Yu
--		DESCRIPTION:	Remove selectTagLoopCallbackFunc and all RFID-related code
--
-- DESIGNER:		Henry Ho
--
-- PROGRAMMER:		Henry Ho
--
-- NOTES:
-- This controller class should be instantiated at the application session level in order to control physical layer
-- functions. This controller class can open ports, close open ports, reset COM port configurations, and handle
-- messages in connection mode. This controller also handles API calls to SkyeTek RFID reader.
----------------------------------------------------------------------------------------------------------------------*/
class CommController {
private:
	char OUTPUT_BUFFER[1] = { 0 };
	HANDLE deviceConnectedEvent = CreateEvent(
		NULL,
		TRUE,
		FALSE,
		TEXT("InitializeReader")
	);

	DisplayService* displayService;
	BOOL isComActive = false;
	VOID drawSingleCharToWindow(char input);
	VOID handleWrite(WPARAM* input);

	// COM Port variables
	COMMCONFIG commConfig;
	HANDLE commHandle;
	COMMPROP commProp;
	LPCWSTR commPortName;

public:


	CommController() {};
	CommController(DisplayService* disp) : displayService(disp) {
		commConfig.dwSize = sizeof(COMMCONFIG);
		commConfig.wVersion = 0x100;
		commPortName = TEXT("COM1");
	};
	HANDLE getDeviceConnectedEvent() inline const { return this->deviceConnectedEvent; };
	DisplayService* getDisplayService() { return this->displayService; };
	VOID closePort();
	VOID handleParam(WPARAM* wParam);
	VOID initializeConnection(LPCWSTR portName);
	VOID setCommConfig(LPCWSTR portName);
	DWORD handleRead(LPVOID input);
	VOID drawBufferToWindow(const char* input, char delimiter);
	VOID writeDataToPort(const char* frame);

	// Below are inline functions for this class

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	getIsComActive
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	BOOL getIsComActive(void)
	--
	-- RETURNS:		BOOL
	--
	-- NOTES:
	-- Call this function to get the connection's state
	----------------------------------------------------------------------------------------------------------------------*/
	BOOL getIsComActive() {
		return isComActive;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	setIsComActive
	--
	-- DATE:		Oct 14, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	 setIsComActive(bool state)
	--					bool state: state to set the current active state of the comm
	----------------------------------------------------------------------------------------------------------------------*/
	void setIsComActive(bool state) {
		isComActive = state;
	}
};
