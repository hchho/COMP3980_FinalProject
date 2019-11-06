#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <comdef.h>
#include <stdio.h>
#include "key_press.h"
#include "error_codes.h"
#include "ErrorHandler.h"
#include "DisplayService.h"
#include "SkyeTekAPI.h"
#include "SkyeTekProtocol.h"
#include <map>

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
--					BOOL initializeRFID(void)
--					VOID drawBufferToWindow(const char* input)
--					BOOL getIsComActive(void)
--					LPSKYETEK_READER getRFIDReader(void)
--					void setRFIDReader(LPSKYETEK_READER r)
--					LPSKYETEK_DEVICE* getDevices(void) const
--					LPSKYETEK_READER* getReaders(void) const
--					unsigned int getNumDevices(void) const
--					unsigned int getNumReaders(void) const
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
	const int NUMBER_OF_TRIES = 10;
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

	// SkyeTek variables
	LPSKYETEK_READER reader;
	LPSKYETEK_DEVICE* devices = NULL;
	LPSKYETEK_READER* readers = NULL;
	unsigned int numDevices;
	unsigned int numReaders;
	map <_bstr_t, int> tagsRead;

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
	void processTag(LPSKYETEK_TAG lpTag);
	static unsigned char selectTagLoopCallbackFunc(LPSKYETEK_TAG lpTag, void* user);

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
	-- FUNCTION:	getRFIDReader
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	BOOL getRFIDReader(void)
	--
	-- RETURNS:		BOOL
	--
	-- NOTES:
	-- Call this function to get the active SkyeTek reader
	----------------------------------------------------------------------------------------------------------------------*/
	LPSKYETEK_READER getRFIDReader() {
		return reader;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	setRFIDReader
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	void setRFIDReader(LPSKYETEK_READER r)
	--					LPSKYETEK_READER r: the reader to set
	--
	-- RETURNS:		void
	--
	-- NOTES:
	-- Call this function set the reader
	----------------------------------------------------------------------------------------------------------------------*/
	void setRFIDReader(LPSKYETEK_READER r) {
		reader = r;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	getDevices
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	LPSKYETEK_DEVICE* getDevices(void) const
	--
	-- RETURNS:		LPSKYETEK_DEVICE*
	--
	-- NOTES:
	-- Call this function to get all the devices detected by SkyeTek_DiscoverDevices
	----------------------------------------------------------------------------------------------------------------------*/
	LPSKYETEK_DEVICE* getDevices() const {
		return devices;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	getReaders
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	LPSKYETEK_READER* getReaders(void) const
	--
	-- RETURNS:		LPSKYETEK_READER*
	--
	-- NOTES:
	-- Call this function to get the readers detected by SkyeTek_DiscoverReaders
	----------------------------------------------------------------------------------------------------------------------*/
	LPSKYETEK_READER* getReaders() const {
		return readers;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	getNumDevices
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	unsigned int getNumDevices(void)
	--
	-- RETURNS:		unsigned int
	--
	-- NOTES:
	-- Call this function to get the number of devices discovered by SkyeTek_DiscoverDevices
	----------------------------------------------------------------------------------------------------------------------*/
	unsigned int getNumDevices() const {
		return numDevices;
	}

	/*------------------------------------------------------------------------------------------------------------------
	-- FUNCTION:	getNumReaders
	--
	-- DATE:		Oct 11, 2019
	--
	-- REVISIONS:	(N/A)
	--
	-- DESIGNER:	Henry Ho
	--
	-- PROGRAMMER:	Henry Ho
	--
	-- INTERFACE:	unsigned int getNumReaders(void)
	--
	-- RETURNS:		unsigned int
	--
	-- NOTES:
	-- Call this function to get the number of devices discovered by SkyeTek_DiscoverReaders
	----------------------------------------------------------------------------------------------------------------------*/
	unsigned int getNumReaders() const {
		return numReaders;
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
