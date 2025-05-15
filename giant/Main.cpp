#include "Timer.h"
#include "Rotor.h"
#include "CmdParse.h"
#include <fstream>
#include <string>
#include <string.h>
#include <stdexcept>
#include <cassert>
#include <algorithm>

#define RELEASE "0.02 (14may25)"
// #define SEARCH_COMPRESSED 0
using namespace std;
// int giantup = 0;
// char myx, myy;
bool should_exit = false;
// #define COIN_BTC 1
// ----------------------------------------------------------------------------
void usage()
{
	printf("CPU-Roter [OPTIONS...] [TARGETS]\n");
	printf("Where TARGETS is one address/xpoint, or multiple hashes/xpoints file\n\n");
	printf("-h, --help                               : this message\n");
	printf("-o, --out FILE                           : Write keys to FILE, default: Found.txt\n");
	printf("-m, --mode MODE                          : Specify search mode where MODE is\n");
	printf("--range KEYSPACE                         : Specify the range:\n");

}

// ----------------------------------------------------------------------------

void getInts(string name, vector<int>& tokens, const string& text, char sep)
{

	size_t start = 0, end = 0;
	tokens.clear();
	int item;

	try {

		while ((end = text.find(sep, start)) != string::npos) {
			item = std::stoi(text.substr(start, end - start));
			tokens.push_back(item);
			start = end + 1;
		}

		item = std::stoi(text.substr(start));
		tokens.push_back(item);

	}
	catch (std::invalid_argument&) {

		printf("  Invalid %s argument, number expected\n", name.c_str());
		usage();
		exit(-1);

	}

}

// ----------------------------------------------------------------------------

//int parseSearchMode(const std::string& s)
//{
//	std::string stype = s;
//	std::transform(stype.begin(), stype.end(), stype.begin(), ::tolower);
//
//	if (stype == "xpoint") {
//		return SEARCH_MODE_SX;
//	}
//	printf("  Invalid search mode format: %s", stype.c_str());
//	usage();
//	exit(-1);
//}

// ----------------------------------------------------------------------------

//int parseCoinType(const std::string& s)
//{
//	std::string stype = s;
//	std::transform(stype.begin(), stype.end(), stype.begin(), ::tolower);
//
//	if (stype == "btc") {
//		return COIN_BTC;
//	}
//
//	printf("  Invalid coin name: %s", stype.c_str());
//	usage();
//	exit(-1);
//}

// ----------------------------------------------------------------------------

bool parseRange(const std::string& s, Int& start, Int& end)
{
	size_t pos = s.find(':');

	if (pos == std::string::npos) {
		start.SetBase16(s.c_str());
		end.Set(&start);
		end.Add(0xFFFFFFFFFFFFULL);
	}
	else {
		std::string left = s.substr(0, pos);

		if (left.length() == 0) {
			start.SetInt32(1);
		}
		else {
			start.SetBase16(left.c_str());
		}

		std::string right = s.substr(pos + 1);

		if (right[0] == '+') {
			Int t;
			t.SetBase16(right.substr(1).c_str());
			end.Set(&start);
			end.Add(&t);
		}
		else {
			end.SetBase16(right.c_str());
		}
	}

	return true;
}
#if 0
#ifdef WIN64
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
		//printf("\n\nCtrl-C event\n\n");
		should_exit = true;
		return TRUE;

	default:
		return TRUE;
	}
}
#endif
#endif

int main(int argc, char** argv)
{
	// Global Init
	Timer::Init();
//	rseed(Timer::getSeed32());
		
//	bool gpuEnable = false;
//	bool gpuAutoGrid = true;
	int compMode = 0;
//	vector<int> gpuId = { 0 };
//	vector<int> gridSize;
	int nbit2 = 0;
//	int display = 1;
	int zet = 0;
	string outputFile = "Found.txt";

	string inputFile = "";	// for both multiple hash160s and x points
//	string address = "";	// for single address mode
	string xpoint = "";		// for single x point mode
	std::vector<unsigned char> myx;
	std::vector<unsigned char> myy;
//	std::vector<unsigned char> hashORxpoint;
	bool singleAddress = false;
	int nbCPUThread = Timer::getCoreNumber();

	bool tSpecified = false;
//	bool useSSE = true;
	uint32_t maxFound = 1024 * 64 * 1024;

//	uint64_t rKey = 0;
	int next = 0;
	Int rangeStart;
	Int rangeEnd;
	rangeStart.SetInt32(0);
	rangeEnd.SetInt32(0);
	Int myblocksize;
	myblocksize.SetInt32(0);

//	int searchMode = 0;
//	int coinType = COIN_BTC;

//	hashORxpoint.clear();

	// cmd args parsing
	CmdParse parser;

	parser.add("-x", "--x", true);
	parser.add("-y", "--y", true);
	parser.add("-h", "--help", false);
	parser.add("-c", "--check", false);
	parser.add("-l", "--list", false);
	parser.add("-u", "--uncomp", false);
	parser.add("-b", "--both", false);
	parser.add("-g", "--gpu", false);
	parser.add("", "--gpui", true);
	parser.add("", "--gpux", true);
	parser.add("-t", "--thread", true);
	parser.add("-i", "--in", true);
	parser.add("-o", "--out", true);
	parser.add("-m", "--mode", true);
	parser.add("", "--coin", true);
	parser.add("", "--range", true);
	parser.add("-v", "--version", false);
	parser.add("-n", "--next", true);
	parser.add("-z", "--zet", true);
	parser.add("-d", "--display", true);
	if (argc == 1) {
		usage();
		return 0;
	}
	try {
		parser.parse(argc, argv);
	}
	catch (std::string err) {
		printf("  Error: %s\n", err.c_str());
		usage();
		exit(-1);
	}
	std::vector<OptArg> args = parser.getArgs();

	for (unsigned int i = 0; i < args.size(); i++) {
		OptArg optArg = args[i];
		std::string opt = args[i].option;

		try {
			if (optArg.equals("-h", "--help")) {
				usage();
				return 0;
			}

			else if (optArg.equals("-t", "--thread")) {
				nbCPUThread = std::stoi(optArg.arg);
				tSpecified = true;
			}
			else if (optArg.equals("-o", "--out")) {
				outputFile = optArg.arg;
			}

			else if (optArg.equals("", "--range")) {
				std::string range = optArg.arg;
				parseRange(range, rangeStart, rangeEnd);
			}
			else if (optArg.equals("-x", "--x")) {
			//	char x = optArg.arg;

				unsigned char xpbytes[32];
				xpoint = optArg.arg[0];
				Int* xp = new Int();
				xp->SetBase16(xpoint.c_str());
				xp->Get32Bytes(xpbytes);
				for (int i = 0; i < 32; i++)
					myx.push_back(xpbytes[i]);
				delete xp;
				if (myx.size() != 32) {
					printf("  Error: %s\n", "  Invalid xpoint");
					usage();
					return -1;
				}

			}
			else if (optArg.equals("-y", "--y")) {
				unsigned char xpbytes[32];
				xpoint = optArg.arg[0];
				Int* xp = new Int();
				xp->SetBase16(xpoint.c_str());
				xp->Get32Bytes(xpbytes);
				for (int i = 0; i < 32; i++)
					myy.push_back(xpbytes[i]);
				delete xp;
				if (myy.size() != 32) {
					printf("  Error: %s\n", "  Invalid xpoint");
					usage();
					return -1;
				}
			}
			else if (optArg.equals("-v", "--version")) {
				printf("Josh-giant v" RELEASE "\n");
				return 0;
			}
		}
		catch (std::string err) {
			printf("Error: %s\n", err.c_str());
			usage();
			return -1;
		}
	}

	// Parse operands
	std::vector<std::string> ops = parser.getOperands();

	if (ops.size() == 0) {
		// read from file
		if (inputFile.size() == 0) {
			printf("  Error: %s\n", "  Missing arguments");
			usage();
			return -1;
		}

	}
	else {
		// read from cmdline
		if (ops.size() != 1) {
			printf("  Error: %s\n", "  Wrong args or more than one address or xpoint are provided, use inputFile for multiple addresses or xpoints");
			usage();
			return -1;
		}

	}

	struct console
	{
		console(unsigned width, unsigned height)
		{
			SMALL_RECT r;
			COORD      c;
			hConOut = GetStdHandle(STD_OUTPUT_HANDLE);
			if (!GetConsoleScreenBufferInfo(hConOut, &csbi))
				throw runtime_error("  You must be attached to a human.");

			r.Left =
				r.Top = 0;
			r.Right = width - 1;
			r.Bottom = height - 1;
			SetConsoleWindowInfo(hConOut, TRUE, &r);

			c.X = width;
			c.Y = height;
			SetConsoleScreenBufferSize(hConOut, c);
		}

		~console()
		{
			SetConsoleTextAttribute(hConOut, csbi.wAttributes);
			SetConsoleScreenBufferSize(hConOut, csbi.dwSize);
			SetConsoleWindowInfo(hConOut, TRUE, &csbi.srWindow);
		}

		void color(WORD color = 0x07)
		{
			SetConsoleTextAttribute(hConOut, color);
		}

		HANDLE                     hConOut;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
	};
	int color = 14;
	int bok1 = 220;
	int bok2 = 1000;

	console con(bok1, bok2);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	printf("\n");
	printf("  Josh-giant v" RELEASE "\n");
	printf("\n");

	nbit2 += nbCPUThread;

	printf("  OUTPUT FILE  : %s\n", outputFile.c_str());
	
//	myx = fux.c_str();

		Rotor* v;

		int mycount = 0;
//			int maxcount = 1024;
	int maxcount = 10;
		myblocksize = 0x1000000000000000;
		printf(" myblocksize %s \n", myblocksize.GetBase16().c_str());
		v = new Rotor(myx,myy, compMode, outputFile, maxFound, nbit2, next, zet, rangeStart.GetBase16(), rangeEnd.GetBase16(), should_exit);

		while (mycount < maxcount)
		{
		v->Search(1, should_exit);
rangeStart.Add(&myblocksize);
rangeEnd.Add(&myblocksize);	
v->rangeStart.SetBase16(rangeStart.GetBase16().c_str());
v->rhex.SetBase16(rangeStart.GetBase16().c_str()); // remove as we dont need/ want random
v->rangeEnd.SetBase16(rangeEnd.GetBase16().c_str());
v->rangeDiff2.Set(&v->rangeEnd);
v->rangeDiff2.Sub(&v->rangeStart);
v->rangeDiffcp.Set(&v->rangeDiff2);
v->rangeDiffbar.Set(&v->rangeDiff2);

// printf("  my count   : %d\n", mycount);
		mycount++;
	}

		v->output2( v->myoutputstr);
delete v;

		printf("\n\n  BYE\n");
		return 0;

}