#include "StateController.h"
#include "ControlCodes.h"
void StateController::parseDataFrame(char *frame)
{
	//CRC on frame using byte 1018-1022

	//Read header
	if(sync)
	//Read Data


}

void StateController::drawBufferToWindow(const char *buff)
{
	drawStringBuffer(buff);
}


void StateController::handleInput(char* input)
{
	switch (state) {
	/* Send States*/
	case PREP_TX:	
		if(ErrorHandler::verifyControl(input, &ACK0))		
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
