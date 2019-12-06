#ifndef STATISTICS_H
#define STATISTICS_H

class Statistics {
public :
	static Statistics& getInstance() {
		static Statistics instance;
		return instance;
	}
private:
	// Private default Constructor for getInstance to call
	Statistics() :
		AckCount(0),
		EnqCount(0),
		ReqCount(0),
		PacketSent(0),
		PacketSentSuccess(0);
		BitErrorRate(0)
		{};

public:
	static DWORD WINAPI drawStatistics(LPVOID param) {
		while (1) {

		}
		return 0;
	};
	int AckCount;
	int EnqCount;
	int ReqCount;
	int PacketSent;
	int BitErrorRate;
	
};
#endif