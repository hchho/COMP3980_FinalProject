#include "StateController.h"
#include "ControlCodes.h"
#include "CommController.h"
#include "SessionService.h"
#include "StateControllerHelper.h"

void StateController::parseDataFrame(char* frame)
{
	//CRC on frame using byte 1018-1022

	//Read header
	//if (sync)
		//Read Data


}

void StateController::drawBufferToWindow(const char* buff)
{
	serv->drawStringBuffer(buff);
}

/* Thread function that will be passed into the writing thread. Infinitely loop while connected*/
DWORD StateController::handleProtocolWriteEvents() {
	DWORD indexOfSignaledEvent;

	while (comm->getIsComActive()) {
		switch (getState()) {
		case STATES::IDLE:
			// Two possible handles to be signaled: IDLE_RECEIVE_ENQ or IDLE_FILE_INPUT 
			indexOfSignaledEvent = WaitForMultipleObjects(EVENT_COUNTS, getEvents()->handles, FALSE, INFINITE);
			if (indexOfSignaledEvent == 0) {
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				setState(PREP_TX);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
			}
			else {
				setState(PREP_RX);
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
				setState(RTR);
			}
			break;
		case STATES::RTR:
			// Three possible handles to be signaled: RTR_FILE_INPUT, RTR_RECEIVE_FRAME, RTR_RECEIVE_EOT
			indexOfSignaledEvent = WaitForMultipleObjects(EVENT_COUNTS, getEvents()->handles, FALSE, INFINITE);
			// I feel like this should be a bool here to send an REQ instead of an ACK when signalling the fact that we have something in our output buffer
			// If it's an event we can't should be 
			if (indexOfSignaledEvent == 5) {
				setState(RX);
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
				setState(RTR);
			}
			else if (indexOfSignaledEvent == 6) {
				setState(RX);
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
				setState(RTR);
			}
			else {
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				setState(IDLE);
			}
			break;
		case STATES::RTS:
			// Set the event for an empty output buffer and set the state to idle after sending an EOT
			if (outputBuffer.empty()) {
				SetEvent(getEvents()->handles[indexOfSignaledEvent]);
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				setState(IDLE);
			}
			else {
				// Two possible handles to be signaled: TX_RECEIVE_ACK ,TX_RECEIVE_REQ
				int errorCounter = 0;
				int resentCounter = 0;
				while (errorCounter++ < 3) {
					// RELIES ON THE READING THREAD TO CALL outputBuffer.pop() when an ACK/REQ is received
					sendFrameToCommController(outputBuffer.front());
					setState(TX);
					indexOfSignaledEvent = WaitForMultipleObjects(EVENT_COUNTS, getEvents()->handles, FALSE, INFINITE);
					if (indexOfSignaledEvent != WAIT_TIMEOUT) {
						break;
					}
					setState(RTS); // following protocol, need to set back to RTS
				}
				if (indexOfSignaledEvent == 3) {
					setState(RTS);
					continue;
				}
				else if (indexOfSignaledEvent == 4) {
					setState(RTS);
					releaseTX = !releaseTX;
					// set timeout to go to idle; also need to reset releaseTX to false when timeout fires
				}
				if (resentCounter++ == 3 && indexOfSignaledEvent == WAIT_TIMEOUT) {
					setState(IDLE);
				}
			}
			break;
		case STATES::PREP_TX:
			indexOfSignaledEvent = WaitForSingleObject(getEvents()->handles[2], INFINITE);
			setState(RTS);
			ResetEvent(getEvents()->handles[2]);
			break;

		default:
			break;
		}

	}
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	sendFrameToCommController
--
-- DATE:		Nov 26, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Michael Yu
--
-- PROGRAMMER:	Michael Yu
--
-- INTERFACE:
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to write the next frame in the output buffer to the serial port. This function relies on the StateHelper
-- to package the frame, given only the pointer to the data stored in the output buffer. This function
-- calls the CommController to perform the writing to the file (port).
----------------------------------------------------------------------------------------------------------------------*/
void StateController::sendFrameToCommController(std::string data) {
	std::string frame = sHelper->buildFrame(data);
	comm->writeFrameToPort(frame);
}

void StateController::handleInput(char* input)
{
	// Depending on state figure out what to expect and if correct ack then signal event
	switch (state) {
	case TX:
		// Expect a REQ or ACK synch bit will be handled in statecontroller 2 bytes
		// Method with logic to handle
		// Verifies based on state  checks for synch bit as well
		//Received AC0
		if (verifyInput(input) == 1) {
			SetEvent(events->handles[3]);
			outputBuffer.pop();
		}
		if (verifyInput(input) == 2) {
			SetEvent(events->handles[4]);
			outputBuffer.pop();
		}
			//SetEvent()
		break;

	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		// Currently just expect an ACk either one will work
		if (verifyInput(input))
			SetEvent(events->handles[2]);
		break;

	case IDLE:
		//Expect a ENQ and only an ENQ Control Code only
		if (verifyInput(input))
			SetEvent(events->handles[1]);
		break;

	case RTR:

		//Is it an EOT
		if (verifyInput(input)){
			SetEvent(events->handles[7]);
		} else {
			//if(CRC Frame) should quick fail if other control character
			//	Parse Frame
			// Output Pop array also remember to delete pointers as they are dynamically allocated
			//serv->drawStringBuffer(input);
			sess->writeToFile(input);
			SetEvent(events->handles[6]);
		}
		break;
	}
}


int StateController::verifyInput(char* input) {
	switch (state) {
	case TX:
		// Expect a REQ or ACK synch bit will be handled in statecontroller 2 bytes
		// Method with logic to handle
		// TODO: check to make sure this is standardized
		// In TX state method returns 1 for ack, or 2 if Req is received, else 0 for false
		if (synch)
			return strncmp(input, &ACK1, 2) == 0 ? 1 : strncmp(input, &REQ1, 2) == 0 ? 2 : 0;
		else
			return strncmp(input, &ACK0, 2) == 0 ? 1 : strncmp(input, &REQ0, 2) == 0 ? 2 : 0;
	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		// Currently just expect an ACK either one will work
		return strncmp(input, &ACK0, 2) || strncmp(input, &ACK1, 2);
	case IDLE:
		//Expect a ENQ and only an ENQ Control Code only
		return strncmp(input, &ENQ, 2) == 0;
	case RTR:
		//returns true if EOT is seen false else Flase? what if it's not an eot and an  or any other control code
		return strncmp(input, &EOT, 2) == 0;
	}
	return 0;
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	sendCommunicationMessageToCommController
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
void StateController::sendCommunicationMessageToCommController(DWORD event) {
	switch (event) {
	case 0: //IDLE_FILE_INPUT
		if (state == IDLE) {
			comm->writeControlMessageToPort(&ENQ);
			setState(PREP_TX);
		}
		break;
	case 5: //RTR_FILE_INPUT
			// This is when we have a file to send Shouldn't be sending anyhting only changing our Reqs to ACKS
		if (state == IDLE) {
			comm->writeControlMessageToPort(&ENQ);
			setState(PREP_TX);
		}
		else if (state == RX) {
			comm->writeControlMessageToPort(&REQ0);
		}
		break;
	case 1: //IDLE_RECEIVE_ENQ
		if (outputBuffer.size() == 0) { //output buffer is empty
			comm->writeControlMessageToPort(&ACK0); // This should either be ACK0 or ACK1
		}
		break;
	case 6: //RTR_RECEIVE_FRAME
		//Perform CRC Validation on received data
		if (true) { // frame is valid
			comm->writeControlMessageToPort(&ACK0); // This should either be ACK0 or ACK1
		}
		else {
			inputBuffer = { 0 }; // Clear input buffer. Discard frame
		}
		break;
	case 9: //RTS_DONE_SENDING 
		if (state == RTS) {
			comm->writeControlMessageToPort(&EOT);
			setState(IDLE);
		}
		break;
	default:
		return;
	}
}

void StateController::handleControlCode(char* code) {

}