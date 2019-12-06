#pragma once
#include <Windows.h>

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		Events.h-	A struct that holds all Events within the system. Several handles are created
								for each logical grouping of events that are related in signalling.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- DATE:			Nov 29, 2019
-- REVISIONS:
--		NA
--
-- DESIGNER:		Michael Yu
--
-- PROGRAMMER:		Michael Yu
--
-- NOTES:
-- This class holds all the events that are possible within the system. The events correspond to those denoted witin the 
-- protocol: Manifest der Data Kommunistischen Partei.
----------------------------------------------------------------------------------------------------------------------*/

constexpr static int EVENT_COUNTS = 10;
constexpr static int ACKNOWLEDGEMENT_HANDLES_COUNT = 3;


struct Events {

	HANDLE IDLE_FILE_INPUT = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE IDLE_RECEIVE_ENQ = CreateEvent(NULL, FALSE, FALSE, NULL);

	// To set state to RTS
	HANDLE PREP_TX = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE TX_RECEIVE_ACK = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE TX_RECEIVE_REQ = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE TX_EMPTY_OUTPUT = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE RTR_FILE_INPUT = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE RTR_RECEIVE_FRAME = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE RTR_RECEIVE_EOT = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE RTS_DONE_SENDING = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE handles[EVENT_COUNTS] = {
		IDLE_FILE_INPUT,	//0
		IDLE_RECEIVE_ENQ,	//1
		PREP_TX,			//2
		TX_RECEIVE_ACK,		//3
		TX_RECEIVE_REQ,		//4
		RTR_FILE_INPUT,		//5
		RTR_RECEIVE_FRAME,	//6
		RTR_RECEIVE_EOT,	//7
		TX_EMPTY_OUTPUT,	//8
		RTS_DONE_SENDING,	//9
	};

	HANDLE acknowledgementHandles[ACKNOWLEDGEMENT_HANDLES_COUNT] = {
		TX_RECEIVE_ACK,		//0
		TX_RECEIVE_REQ,		//1
		RTS_DONE_SENDING,
	};

	Events() {};

};