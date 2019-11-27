#ifndef STATES_H
#define STATES_H
enum STATES {

	/* IDLE STATES */
	IDLE,
	
	/* SEND STATES */
	PREP_TX,	// Sent ENQ waiting for ACK
	RTS,		// Received ACK send frame
	TX,			// Frame sent waiting for ACK
	SW_RX,		// Received REQ Switch to TX

	/* READ STATES */
	PREP_RX,	// Received ENQ sending ACK
	RTR,		// Waiting for frame
	RX,			// Received Frame sending ack
	SW_TX		// Has input to Buffer will switch to sending REQS as ACKS

};

#endif /* STATES_H */