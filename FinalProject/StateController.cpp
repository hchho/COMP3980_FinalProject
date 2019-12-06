#include "StateController.h"
#include "ControlCodes.h"
#include "CommController.h"
#include "SessionService.h"
#include "StateControllerHelper.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		StateController.cpp - A controller class that handles all the states in our system related to the data-link 
--										protocol and manages all events that are created.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--					
--					DWORD handleProtocolWriteEvents()
--					void handleControlCode(char* code)
--					void handleInput(char* input)
--					void drawBufferToWindow(const char* buff)
--					void parseDataFrame(char* frame)
--					void parseDataFrame(char* frame)
--					void sendCommunicationMessageToCommController(DWORD event)
--					void sendFrameToCommController(std::string data)
--					int verifyInput(char* input)
--
-- DATE:			Dec 06, 2019
--
-- REVISIONS:
--					N/A
--
-- DESIGNER:		Michael Yu, Henry Ho, Albert Liu
--
-- PROGRAMMER:		Michael Yu, Henry Ho, Albert Liu
--
-- NOTES:
-- The class is responsible for maintaining and updating the state of the current system in relation to the protocol.
-- A singular instance of this class lives within the system when the application connects. All events are created and live
-- only within the StateController. Writing and reading of data in the physical layer are delegated to the CommController 
-- to perform the appropriate task. 
----------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	parseDataFrame
--
-- DATE:		Dec 05, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Albert Liu
--
-- PROGRAMMER:	Albert Liu
--
-- INTERFACE:	VOID parseDataFrame(char* frame)
--						char* frame - the frame that will be unpacked 
--
-- RETURNS:		void
--
-- NOTES:
-- Calling this function on a frame will unpack the frame into its respective segments: SYN, STX, Data, CRC, and EOT.
----------------------------------------------------------------------------------------------------------------------*/
void StateController::parseDataFrame(char* frame)
{
	//CRC on frame using byte 1018-1022

	//Read header
	//if (sync)
		//Read Data


}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleProtocolWriteEvents
--
-- DATE:		Dec 05, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Michael Yu
--
-- PROGRAMMER:	Michael Yu
--
-- INTERFACE:	DWORD handleProtocolWriteEvents()
--
-- RETURNS:		void
--
-- NOTES:
-- Thread function that handles all writing events within the program. The system will infinitely loop while the program is
-- running. The system immediately starts off in IDLE until a valid event(ENQ, FILE_INPUT, RTR_FILE_INPUT) is signaled to the system.
-- Based on the current state of the system and the event that is signaled, the system will send the corresponding data to the 
-- writing port. Random timeout of the system is generated here when the current system is sending frames and receives consecutive REQs.
----------------------------------------------------------------------------------------------------------------------*/
DWORD StateController::handleProtocolWriteEvents() {
	DWORD indexOfSignaledEvent;
	DWORD indexOfSignaledEvent2;
	std::random_device rdm;
	std::mt19937 generator(rdm());

	while (comm->getIsComActive()) {
		switch (getState()) {
		case STATES::IDLE:
			// Two possible handles to be signaled: IDLE_RECEIVE_ENQ or IDLE_FILE_INPUT 
			indexOfSignaledEvent = WaitForMultipleObjects(EVENT_COUNTS, getEvents()->handles, FALSE, 1000);
			// Receive File input
			if(indexOfSignaledEvent == 0) {
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				setState(PREP_TX);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
				// Wait for ACK0/ACK1/ENQ from reading side to be signalled for timeout -- StateController 170
			}
			else if (indexOfSignaledEvent == 1) { // receive ENQ
				setState(PREP_RX);
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
				setState(RTR);
			}
			else if (!outputBuffer.empty()) {
				serv->drawStringBuffer("Bidding for channel and output buffer not empty", 'n');
				sendCommunicationMessageToCommController(0); // sending ENQ
				setState(PREP_TX);
				ResetEvent(getEvents()->handles[0]);
			}
			break;
		case STATES::RTR:
			// Three possible handles to be signaled: RTR_FILE_INPUT, RTR_RECEIVE_FRAME, RTR_RECEIVE_EOT
			indexOfSignaledEvent = WaitForMultipleObjects(EVENT_COUNTS, getEvents()->handles, FALSE, 4500);
			if (indexOfSignaledEvent == 7) { // receive EOT 
				setState(IDLE);
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
			}
			else if (indexOfSignaledEvent == 5) { // receive file input
				setState(RX);
				sendCommunicationMessageToCommController(5); // send REQ
				ResetEvent(getEvents()->handles[indexOfSignaledEvent]);
				setState(RTR);
			}
			else if (indexOfSignaledEvent == 6) { // receive frame
				setState(RX);
				int indexToSend = outputBuffer.empty() ? 6 : 5; // If the output buffer is emtpy, we send an ACK, otherwise, we send a REQ
				sendCommunicationMessageToCommController(indexToSend); 
				ResetEvent(getEvents()->handles[indexToSend]);
				setState(RTR);
			}
			else {
				serv->drawStringBuffer("Timed out from RTR", 'n');
				sendCommunicationMessageToCommController(indexOfSignaledEvent);
				setState(IDLE);
			}
			break;
		case STATES::RTS:
			// Set the event for an empty output buffer and set the state to idle after sending an EOT
			if (outputBuffer.empty()) {
				sendCommunicationMessageToCommController(9);
				// RESET BOTH FILE_INPUT_IDLE && FILE_INPUT_RTR
				ResetEvent(getEvents()->handles[0]); // Reset to IDLE from IDLE_FILE_INPUT because finished sending
				ResetEvent(getEvents()->handles[5]); // Reset to IDLE from RTR_FILE_INPUT because finished sending
				ResetEvent(getEvents()->handles[7]);
				DisplayService::displayMessageBox("RTS Sending EOT Finished sending");
				setState(IDLE);
			}
			else {
				// Two possible handles to be signaled: TX_RECEIVE_ACK ,TX_RECEIVE_REQ
				int errorCounter = 0;
				int resentCounter = 0;
				// Error Repeadtely sends 3 frames or else breaks
				while (errorCounter++ < 3) {
					// RELIES ON THE READING THREAD TO CALL outputBuffer.pop() when an ACK/REQ is received
					sendFrameToCommController(outputBuffer.front());
					setState(TX);
					indexOfSignaledEvent = WaitForMultipleObjects(ACKNOWLEDGEMENT_HANDLES_COUNT, getEvents()->acknowledgementHandles, FALSE, 1500);

					if (indexOfSignaledEvent != WAIT_TIMEOUT) { // We received ACK or REQ
						break;
					}
					serv->drawStringBuffer("Resent a frame", 'n');
					setState(RTS); // following protocol, need to set back to RTS
				}
				// Received an ack
				ResetEvent(getEvents()->acknowledgementHandles[indexOfSignaledEvent]);
				if (indexOfSignaledEvent == 0) { // 0 is ACK
					setState(RTS);
					continue;
				}
				//Received an Req
				else if (indexOfSignaledEvent == 1) { // 1 is REQ
					setState(RTS);
					releaseTX = true;
					continue;
					// set timeout to go to idle; also need to reset releaseTX to false when timeout fires
				}
				else if (indexOfSignaledEvent == 2) {// 2 is send EOT because of REQ 
					releaseTX = false;
					sendCommunicationMessageToCommController(9);
					std::random_device rdm;
					std::mt19937 generator(rdm());
					Sleep(distribution(generator));
					break;
				}

				if (indexOfSignaledEvent == WAIT_TIMEOUT) {
					serv->drawStringBuffer("Timed out from RTS", 'n');
					setState(IDLE);
				}
			}
			break;
		case STATES::PREP_TX:
			indexOfSignaledEvent = WaitForSingleObject(getEvents()->handles[2], 1500);
			if (indexOfSignaledEvent == WAIT_TIMEOUT) {
				serv->drawStringBuffer("Timed out from PREP_TX", 'n');
				setState(IDLE);
				break;
			}
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

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	handleInput
--
-- DATE:		Dec 05, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Albert Liu
--
-- PROGRAMMER:	Albert Liu, Henry Ho, Michael Yu
--
-- INTERFACE:	VOID handleInput(char* input)
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to process the control code input that is received and respond the the corresponding code.
----------------------------------------------------------------------------------------------------------------------*/
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
			outputBuffer.pop();
			SetEvent(events->handles[3]);
			break;
		}
		if (verifyInput(input) == 2) {
			outputBuffer.pop();
			serv->drawStringBuffer("Receiving REQ", 'n');
			if (++reqCounter > 3 && releaseTX) {
				serv->drawStringBuffer("Switching out from REQ", 'n');
				reqCounter = 0;
				SetEvent(events->handles[9]);
				break;
			} 
			SetEvent(events->handles[4]);
		}
		break;
	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		// Currently just expect an ACk either one will work
		if (verifyInput(input) == 0 || verifyInput(input) == 1) {
			SetEvent(events->handles[2]);
		}
		else { // RECEIVED ENQ IN PREP_TX - timeout system for random duration
			std::random_device rdm;
			std::mt19937 generator(rdm());
			serv->drawStringBuffer("Timed out from simultaneous bidding.", 'n');
			DWORD throwawayENQ = WaitForSingleObject(getEvents()->handles[2], distribution(generator));
			setState(IDLE);
		}
		break;
	case IDLE:
		//Expect a ENQ and only an ENQ Control Code only
		if (verifyInput(input))
			SetEvent(events->handles[1]);
		break;

	case RTR:
		//Is it an EOT
		if (verifyInput(input)) {
			ResetEvent(events->handles[5]);
			SetEvent(events->handles[7]);
		}
		else {
			std::string content(input,1024);
			if (ErrorHandler::checksumMatch(content)) {
				sess->writeToFile(content);
				SetEvent(events->handles[6]); // Receive frame in RTR
			}
		}
		break;
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	verifyInput
--
-- DATE:		Nov 27, 2019
--
-- REVISIONS:	(N/A)
--
-- DESIGNER:	Albert Liu
--
-- PROGRAMMER:	Albert Liu, Henry Ho
--
-- INTERFACE:	int StateController::verifyInput(char* input)
--					char* input - represents the value of the control code character that is received by the system
--
-- RETURNS:		int
--
-- NOTES:
-- Call this function to verify the control code received is a valid code during the current state of the system.
----------------------------------------------------------------------------------------------------------------------*/
int StateController::verifyInput(char* input) {
	int i = *(input + 1);
	switch (state) {
	case TX:
		if (i == REQ0) {
			return 2;
		}
		if (i == REQ1) {
			return 2;
		}
		if (i == ACK0) {
			return 1;
		}
		if (i == ACK1) {
			return 1;
		}		
		break;
	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		// Currently just expect an ACK either one will work
		// HANDLE CONDITION FOR ENQ (SIMULTANEOUS BIDDING)
		return i == ACK0 || i == ACK1;
	case IDLE:
		//Expect a ENQ and only an ENQ Control Code only
		return i == ENQ;
	case RTR:
		//returns true if EOT is seen false else Flase? what if it's not an eot and an  or any other control code
		return i == EOT;
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
-- INTERFACE:	void StateController::sendCommunicationMessageToCommController(DWORD event)
--					DWORD event - represents the event that was signalled during the current state in the system
--
-- RETURNS:		void
--
-- NOTES:
-- Call this function to write a control message to the port. Messages that are sent to the port are dependent on the combination of
-- the current state of the system and the event that is currently signaled.
----------------------------------------------------------------------------------------------------------------------*/
void StateController::sendCommunicationMessageToCommController(DWORD event) {
	switch (event) {
	case 0: //IDLE_FILE_INPUT
		if (state == IDLE) {
			comm->writeControlMessageToPort(&ENQ);
			setState(PREP_TX);
		}
		break;
	case 1: //IDLE_RECEIVE_ENQ
		comm->writeControlMessageToPort(&ACK0); // This should either be ACK0 or ACK1
		break;
	case 5: //RTR_FILE_INPUT
	// This is when we have a file to send Shouldn't be sending anyhting only changing our Reqs to ACKS
		if (state == IDLE) {
			comm->writeControlMessageToPort(&ENQ);
			setState(PREP_TX);
		}
		else if (state == RX) {
			comm->writeControlMessageToPort(&REQ0);
			serv->drawStringBuffer("Sending REQ", 'n');
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
		if (state == RTS || state == TX) {
			std::string code;
			for (int i = 0; code.size() < 1024; i++) {
				code += EOT;
			}
			comm->writeFrameToPort(code);
			serv->drawStringBuffer("RTS Done Sending. Sending EOT Finished sending", 'n');
			setState(IDLE);
		}
		break;
	default:
		return;
	}
}

void StateController::handleControlCode(char* code) {

}