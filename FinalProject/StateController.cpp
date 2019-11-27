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
// How would we run into those scenarios?
void StateController::handleInput(char* input)
{
	// Depending on state figure out what to expect and if correct ack then signal event
	switch (state) {
	case TX:
		// Expect a REQ or ACK synch bit will be handled in statecontroller 2 bytes
		// Method with logic to handle
		// Verifies based on state  checks for synch bit as well
		if(verifyInput(input))
			//SetEvent()


		break;
	case PREP_TX:
		// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
		// Currently just expect an ACk either one will work
		if (verifyInput(input))
			//SetEvent()
		break;
	case IDLE:
		//Expect a ENQ and only an ENQ Control Code only
		if (verifyInput(input))
			//SetEvent()
		break;
	case RTR:
		//Expect a data frame
		//Or could be an EOT this is the only Staete that should handle either 1028 bytes or 2 byte response

		//Is it an EOT
		if (verifyInput(input)){}			
		// SetEvent()
		// else it's a data frame
		else {
			//if(CRC Frame) should quick fail if other control character
			//	Parse Frame
		}
		break;
	}
}

boolean StateController::verifyInput(char* input) {
		switch (state) {
		case TX:
			// Expect a REQ or ACK synch bit will be handled in statecontroller 2 bytes
			// Method with logic to handle
			// TODO: check to make sure this is standardized

			if (synch) 
				return strcmp(input, &ACK1) || strcmp(input, &REQ1);
			else 
				return strcmp(input, &ACK0) || strcmp(input, &REQ0);
			
		case PREP_TX:
			// Expect a ACK0 or ACK1 ?to get control of line Control Code Only 2 bytes
			// Currently just expect an ACK either one will work
			return strcmp(input, &ACK0) || strcmp(input, &ACK1);
		case IDLE:
			//Expect a ENQ and only an ENQ Control Code only
			return strcmp(input, &ENQ);
		case RTR:
			//returns true if EOT is seen false else Flase? what if it's not an eot and an  or any other control code
			return strcmp(input, &EOT);
		}
		return true;
}

