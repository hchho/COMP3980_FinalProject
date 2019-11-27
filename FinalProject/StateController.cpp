#include "StateController.h"
#include "ControlCodes.h"
void StateController::parseDataFrame(char* frame)
{
	//CRC on frame using byte 1018-1022

	//Read header
	/*if (sync)*/
		//Read Data


}

void StateController::drawBufferToWindow(const char* buff)
{
	/*drawStringBufferh(buff);*/
}

/* Thread function that will be passed into the writing thread. Infinitely loop while connected*/
void StateController::handleWrite() {
	HANDLE dwEvent{ 0 };

	while (comm->getIsComActive) {
		//if (WaitCommEvent(sessionService->readThread, &dwEvent, 0)) {
		//	// If current state is RTR and reading thread receives a frame, send ACK or REQ
		//	// inside the comm message, we will check if the current system has something to send
		//	if (state == STATES::RTR) {
		//		comm->sendCommunicationMessage(sessionService->getEvents().receivedFrame);
		//	}
		//	else if (state == STATES::RTS) {
		//		// condition for empty write buffer to send EOT
		//		
		//		// condition for frame acknowledged by an ACK
		//		// condition for frame acknowledged by a REQ
		//		// condition for the first initial send
		//		
		//	}

		//}
	}
}




// QUESTION Reset events everytime we receive new input? What happens if we have multiple events that are signalled and haven't been reset? 
// How wwould we run into those scenarios?
void StateController::handleInput(char* input)
{
	// 
	switch (state) {
	case TX:
		// Expect a REQ or ACK synch bit will be handled in statecontroller 2 bytes

		break;
	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		
		break;
	case IDLE:
		//Expect a ENQ and only an ENQ Control Code only
		break;
	case RTR:
		//Expect a data frame
		//Or could be an EOT this is the only Staete that should handle either 1028 bytes or 2 byte response
		break;
	//switch (state) {
	//	/* Send States*/
	//case PREP_TX:
	//	if (ErrorHandler::verifyControl(input, &ACK0))
	//		//Start Send()				
	//		break;
	//case TX:
	//	if (ErrorHandler::verifyControl(input, &ACK0))
	//		//sendNext();

	///* Read States*/
	//case RTR:
	//	if (!ErrorHandler::verifyC(input))
	//		handleControlCode(input);
	//	break;

	//	/* Idle State*/
	//case IDLE:
	//	if (ErrorHandler::verifyCommand(input))

	//		break;
	//default:										// Default case means input has been received during a state that should not receive input	
	//	break;
	//}
}

