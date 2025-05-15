#ifndef ROTORH
#define ROTORH

#include <string>
#include <vector>
#include "SECP256k1.h"
#ifdef WIN64
#include <Windows.h> // nedded???
#endif

#define CPU_GRP_SIZE (4096)

class Rotor;

typedef struct {
	Rotor* obj;
	int  threadId;
	bool isRunning;
	bool hasStarted;
	Int rangeStart;
	Int rangeEnd;

} TH_PARAM;

class Rotor
{

public:
	Rotor(const std::vector<unsigned char>& myx1, const std::vector<unsigned char>& myy, int compMode, const std::string& outputFile, uint32_t maxFound, int nbit2, int next, int zet,
		const std::string& rangeStart, const std::string& rangeEnd, bool& should_exit);

	~Rotor();

	void Search(int nbThread, bool& should_exit);
	void FindKeyCPU(TH_PARAM* p);
//	const std::vector<unsigned char>& myx1;
//	const std::vector<unsigned char>& myy1;
	Int rangeStart8;
	Int rangeStart;
	Int rangeEnd;
	Int rangeDiff;
	Int rangeDiff2;
	Int rangeDiffcp;
	Int rangeDiffbar;
	Int rhex;
	std::string myoutputstr;
	void output2(std::string myoutputstr);
	void output3(std::string myoutputstr);
private:

	void InitGenratorTable();

//	std::string GetHex(std::vector<unsigned char>& buffer);
	void checkSingleXPoint(bool compressed, Int key, int i, Point p1);
//	void output(std::string pAddrHex, std::string pubKey);
std::string myx1;
std::string myy1;
	bool isAlive(TH_PARAM* p);

	bool hasStarted(TH_PARAM* p);

	uint64_t getCPUCount();
	void SetupRanges(uint32_t totalThreads);

	void getCPUStartingKey(Int& tRangeStart, Int& tRangeEnd, Int& key, Point& startP);

	bool MatchXPoint(uint32_t* _h);
//	std::string formatThousands(uint64_t x);
//	char* toTimeStr(int sec, char* timeStr);

	Secp256K1* secp;
	uint64_t counters[256];
	double startTime;

//	int compMode;
//	int searchMode;
	int coinType;

//	bool useGpu;
	bool endOfSearch;
	int nbCPUThread;
//	int nbGPUThread;
	int nbFoundKey;
	uint64_t targetCounter;
	std::string stroka;
	std::string outputFile;
	std::string inputFile;
//	uint32_t hash160Keccak[5];
	uint32_t xpoint[8];
//	bool useSSE;

	Int gir;
	int trk = 0;
	Int razn;

	int minuty;
	int zhdat;
	uint64_t value777;
	uint32_t maxFound;
//	uint64_t rKey;
	int next;
	int zet;
//	int display;
	int nbit2;
//	int gpucores;
	uint64_t lastrKey;
//	uint64_t rKeyCount2;
//	int corres;
	uint8_t* DATA;
//	uint64_t TOTAL_COUNT;
//	uint64_t BLOOM_N;

//#ifdef WIN64
//	HANDLE ghMutex;
//#else
//	pthread_mutex_t  ghMutex;
//#endif

};

#endif // ROTORH
