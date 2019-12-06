
#ifndef STATISTICS_H
#define STATISTICS_H

#include <string>
class Statistics {
private:


	int PacketSent;
	int PacketRejected;
	int PacketAccepted;
	int PacketReceived;

	int AckCount;
	int ReqCount;
	int TimeoutCount;

	DisplayService *disp;
public:
	Statistics(DisplayService* disp) :
		disp(disp),
		PacketSent(0),
		PacketRejected(0),
		PacketAccepted(0),
		AckCount(0),
		ReqCount(0),
		TimeoutCount(0)
	{};

	DWORD drawStatistics() {
		while (1) {
			std::string ackSent = "Ack Sent: " + std::to_string(AckCount);
			std::string reqSent = "Req Count: " + std::to_string(ReqCount);
			std::string timeoutSent = "TimeoutCount: " + std::to_string(TimeoutCount);
			std::string packetSent = "Packet Sent: " + std::to_string(PacketSent);
			std::string packetRejected = "Packet Rejected " + std::to_string(PacketRejected);
			std::string packetAccepted = "Packet Accepted: " + std::to_string(PacketAccepted);

			std::string bitErrorRate = "BitErrorRate: ";

			if (PacketRejected + PacketAccepted) {
				bitErrorRate += std::to_string(8 * 1024 * PacketRejected / (PacketRejected + PacketAccepted));
			}

			disp->drawStringBuffer(ackSent.c_str(),'n');
			disp->drawStringBuffer(reqSent.c_str(), 'n');
			disp->drawStringBuffer(timeoutSent.c_str(), 'n');
			disp->drawStringBuffer(packetSent.c_str(), 'n');
			disp->drawStringBuffer(packetRejected.c_str(), 'n');
			disp->drawStringBuffer(packetAccepted.c_str(), 'n');
			disp->drawStringBuffer(bitErrorRate.c_str(),'m');
			Sleep(1000);
		}
		return 0;
	}

	void incrementPacketSent() { ++PacketSent; };
	void incrementPacketRejected() { ++PacketRejected; };
	void incrementPacketAccepted() { ++PacketAccepted; };
	void incrementAckCount() { ++AckCount; };
	void incrementReqCount() { ++ReqCount; };
	void incrementTimeoutCount() { ++TimeoutCount; };
	
	
};
#endif