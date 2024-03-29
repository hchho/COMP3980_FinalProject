#include "StateController.h"
#include "ControlCodes.h"
#include "CommController.h"
#include "SessionService.h"
#include "StateControllerHelper.h"
#include "Statistics.h"

/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE:		StateController.cpp - A controller class that handles all the states in our system related to the data-link
--										protocol and manages all events that are created.
--
-- PROGRAM:			DumbSerialPortEmulator
--
-- FUNCTIONS:
--
--					DWORD handleProtocolWriteEvents()
--					void handleInput(char* input)
--					void drawBufferToWindow(const char* buff)
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
			// If in IDLE, next send event should always be SYN0
			syncBit = 0;
			// Two possible handles to be signaled: IDLE_RECEIVE_ENQ or IDLE_FILE_INPUT 
			indexOfSignaledEvent = WaitForMultipleObjects(EVENT_COUNTS, getEvents()->handles, FALSE, 1000);
			// Receive File input
			if (indexOfSignaledEvent == 0) {
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
				stats->incrementTimeoutCount();
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
				setState(IDLE);
			}
			else {
				// Two possible handles to be signaled: TX_RECEIVE_ACK ,TX_RECEIVE_REQ
				int errorCounter = 0;
				int resentCounter = 0;
				// Error Repeadtely sends 3 frames or else breaks
				while (errorCounter++ < 3) {
					// RELIES ON THE READING THREAD TO CALL outputBuffer.pop() when an ACK/REQ is received
					sendFrameToCommController(outputBuffer.front(), syncBit);
					setState(TX);
					indexOfSignaledEvent = WaitForMultipleObjects(ACKNOWLEDGEMENT_HANDLES_COUNT, getEvents()->acknowledgementHandles, FALSE, 1500);

					if (indexOfSignaledEvent != WAIT_TIMEOUT) { // We received ACK or REQ
						break;
					}
					stats->incrementPacketSent();
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
					stats->incrementTimeoutCount();
					releaseTX = false;
					sendCommunicationMessageToCommController(9);
					std::random_device rdm;
					std::mt19937 generator(rdm());
					Sleep(distribution(generator));
					break;
				}

				if (indexOfSignaledEvent == WAIT_TIMEOUT) {
					stats->incrementTimeoutCount();
					setState(IDLE);
				}
			}
			break;
		case STATES::PREP_TX:
			indexOfSignaledEvent = WaitForSingleObject(getEvents()->handles[2], 1500);
			if (indexOfSignaledEvent == WAIT_TIMEOUT) {
				stats->incrementTimeoutCount();
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
void StateController::sendFrameToCommController(std::string data, int syncBit) {
	std::string frame = sHelper->buildFrame(data, syncBit);
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
		//Received ACK0 or ACK1
		if (verifyInput(input) == 1) {
			syncBit = syncBit == 0 ? 1 : 0; // flip syncBit if I verified that I received the correct ACK(SYN)
			outputBuffer.pop();
			SetEvent(events->handles[3]);
			break;
		}
		//Received ACK0 or ACK1
		if (verifyInput(input) == 2) {
			syncBit = syncBit == 0 ? 1 : 0; // flip syncBit if I verified that I received the correct REQ(SYN)
			outputBuffer.pop();
			stats->incrementReqCount();
			if (++reqCounter > 3 && releaseTX) {
				reqCounter = 0;
				SetEvent(events->handles[9]);
				break;
			}
			SetEvent(events->handles[4]);
		}
		break;
	case PREP_TX:
		// Expect only an ACK0 to get control of line Control Code Only 2 bytes
		if (verifyInput(input) == 1) {
			SetEvent(events->handles[2]);
		}
		else { // RECEIVED ENQ IN PREP_TX - timeout system for random duration
			std::random_device rdm;
			std::mt19937 generator(rdm());
			stats->incrementTimeoutCount();
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
			
			std::string content(input, 1024);
			if (syncBit == input[0]) {
				if (ErrorHandler::checksumMatch(content)) {
					stats->incrementPacketAccepted();
					sess->writeToFile(content.substr(2,1017)); // Grabs Payload to write to file
					SetEvent(events->handles[6]); 
				}
				else if (syncBit != input[0]) { // check first char of the input data
					stats->incrementPacketRejected();
					syncBit = syncBit == 0 ? 1 : 0; // generate the opposite SYNC and send ACK alongside 
					SetEvent(events->handles[6]);
				}
				else {
					stats->incrementPacketRejected();
				}
			}
			break;
		}
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
-- PROGRAMMER:	Albert Liu, Henry Ho, Michael Yu
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
		if (i == ACK0 && syncBit == SYN0 || i == ACK1 && syncBit == SYN1) {
			return 1;
		}
		else if (i == ACK0 && syncBit == SYN1 || i == ACK1 && syncBit == SYN0) { // Receive flipped ACK for lost ACK state
			// flip SYNCBIT
			syncBit = syncBit == 0 ? 1 : 0; // flip syncBit since I received a flipped ACK
			return 1;
		}
		if (i == REQ0 && syncBit == SYN0 || i == REQ1 && syncBit == SYN1) {
			return 2;
		}
		else if (i == ACK0 && syncBit == SYN1 || i == ACK1 && syncBit == SYN0) { // Receive flipped ACK for lost ACK state
			// flip SYNCBIT
			syncBit = syncBit == 0 ? 1 : 0; // flip syncBit since I received a flipped ACK
			return 2;
		}
		break;
	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		// Currently just expect an ACK either one will work
		// 0 = ack0, 1 = ack1, 2 = ENQ
		return i == ACK0;
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
			comm->writeControlMessageToPort(calculateMSBofControlCode(syncBit), &ENQ);

			setState(PREP_TX);
		}
		break;
	case 1: //IDLE_RECEIVE_ENQ
		stats->incrementAckCount();
		comm->writeControlMessageToPort(calculateMSBofControlCode(syncBit), &ACK0); // This should only be ACK0
		break;
	case 5: //RTR_FILE_INPUT
	// This is when we have a file to send Shouldn't be sending anyhting only changing our Reqs to ACKS
		if (state == IDLE) {
			comm->writeControlMessageToPort(calculateMSBofControlCode(syncBit), &ENQ);
			setState(PREP_TX);
		}
		else if (state == RX) {
			stats->incrementReqCount();
			comm->writeControlMessageToPort(calculateMSBofControlCode(syncBit), &REQ0);
			syncBit = syncBit == 0 ? 1 : 0; // flip syncBit if I verify that I received the correct ACK(SYN)
			
		}
		break;
	case 6: //RTR_RECEIVE_FRAME
		comm->writeControlMessageToPort(calculateMSBofControlCode(syncBit), &ACK0); // This should either be ACK0 or ACK1
		syncBit = syncBit == 0 ? 1 : 0; // flip syncBit if I verify that I received the correct ACK(SYN)
		break;
	case 9: //RTS_DONE_SENDING 
		if (state == RTS || state == TX) {
			std::string code;
			for (int i = 0; code.size() < 1024; i++) {
				code += EOT;
			}
			stats->incrementPacketSent();
			comm->writeFrameToPort(code);
			setState(IDLE);
		}
		break;
	default:
		return;
	}
}


char StateController::calculateMSBofControlCode(int syncBit) {
	return syncBit == 1 ? SYN1 : SYN0; // SYN0 == _NUL, covers all cases for control codes
}