#include "StateController.h"
#include "ControlCodes.h"
void StateController::parseDataFrame(char* frame)
{
	//CRC on frame using byte 1018-1022

	//Read header
	if (sync)
		//Read Data


}

void StateController::drawBufferToWindow(const char* buff)
{
	drawStringBuffer(buff);
}

/* Thread function that will be passed into the writing thread. Infinitely loop while connected*/
void StateController::handleWrite() {
	HANDLE dwEvent{ 0 };

	while (comm->getIsComActive) {
		if (WaitCommEvent(sessionService->readThread, &dwEvent, 0)) {
			// If current state is RTR and reading thread receives a frame, send ACK or REQ
			// inside the comm message, we will check if the current system has something to send
			if (state == STATES::RTR) {
				sendCommunicationMessage(sessionService->getEvents().receivedFrame);
			}
			else if (state == STATES::RTS) {
				// condition for empty write buffer to send EOT
				
				// condition for frame acknowledged by an ACK
				// condition for frame acknowledged by a REQ
				// condition for the first initial send
				
			}

		}
	}
}

void StateController::handleInput(char* input)
{
	switch (state) {
		/* Send States*/
	case PREP_TX:
		if (ErrorHandler::verifyControl(input, &ACK0))
			//Start Send()				
			break;
	case TX:
		if (ErrorHandler::verifyControl(input, &ACK0))
			//sendNext();

	/* Read States*/
	case RTR:
		if (!ErrorHandler::verifyC(input))
			handleControlCode(input);
		break;

		/* Idle State*/
	case IDLE:
		if (ErrorHandler::verifyCommand(input))

			break;
	default:										// Default case means input has been received during a state that should not receive input	
		break;
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	sendCommunicationMessage
--
-- DATE:		Nov 26, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Michael Yu
--
-- PROGRAMMER:	Henry Ho
--
-- INTERFACE:
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to write a control message to the port. 
----------------------------------------------------------------------------------------------------------------------*/
void StateController::sendCommunicationMessage(DWORD event) {
	switch (event) {
	case 0: //IDLE_FILE_INPUT
	case 5: //RTR_FILE_INPUT
		if (state == IDLE) {
			comm->sendMessage(ENQ);
			setState(PREP_TX);
		}
		else if (state == RX) {
			comm->sendMessage(REQ0); 
		}
		break;
	case 1: //IDLE_RECEIVE_ENQ
		if (outputBuffer == nullptr) { //output buffer is empty
			comm->sendMessage(ACK0); // This should either be ACK0 or ACK1
		}
		break;
	case 6: //RTR_RECEIVE_FRAME
		//Perform CRC Validation on received data
		if (true) { // frame is valid
			comm->sendMessage(ACK0); // This should either be ACK0 or ACK1
		}
		else {
			inputBuffer = { 0 }; // Clear input buffer. Discard frame
		}
		break;
	case 8: //RTS_DONE_SENDING 
		if (state == RTS) {
			comm->sendMessage(EOT);
			setState(IDLE);
		}
		break;
	default:
		return;
	}
}
