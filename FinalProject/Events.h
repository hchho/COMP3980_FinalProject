#pragma once
#include <Windows.h>

struct Events {
	HANDLE IDLE_RECEIVE_ENQ{ 0 };
	HANDLE IDLE_FILE_INPUT{ 0 };

	// To set state to RTS
	HANDLE PREP_TX{ 0 };

	HANDLE TX_RECEIVE_ACK{ 0 };
	HANDLE TX_RECEIVE_REQ{ 0 };

	HANDLE RTR_RECEIVE_FRAME{ 0 };
	HANDLE RTR_RECEIVE_EOT{ 0 };
	HANDLE RTR_FILE_INPUT{ 0 }; // what's the difference between this and IDLE_FILE_INPUT?

	HANDLE RTS_DONE_SENDING{ 0 };
};