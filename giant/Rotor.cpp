#include "Rotor.h"
#include "GmpUtil.h"
#include <fstream>
#include "IntGroup.h"
#include "Timer.h"
#include <cstring>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "SECP256k1.h"
#include <iomanip>

using namespace std;

Point Gn[CPU_GRP_SIZE / 2];
Point _2Gn;

// --------------------------------------------------------------------

Rotor::Rotor(const std::vector<unsigned char>& myx1, const std::vector<unsigned char>& myy1, int compMode, const std::string& outputFile,  uint32_t maxFound, int nbit2, int next, int zet,
	const std::string& rangeStart, const std::string& rangeEnd, bool& should_exit)
{
	this->outputFile = outputFile;
	this->maxFound = maxFound;
	this->zet = zet;
	this->stroka = stroka;
	this->rangeStart.SetBase16(rangeStart.c_str());
	this->rhex.SetBase16(rangeStart.c_str());
	this->rangeEnd.SetBase16(rangeEnd.c_str());
	this->rangeDiff2.Set(&this->rangeEnd);
	this->rangeDiff2.Sub(&this->rangeStart);
	this->rangeDiffcp.Set(&this->rangeDiff2);
	this->rangeDiffbar.Set(&this->rangeDiff2);
	this->targetCounter = 1;
	this->nbit2 = nbit2;
	secp = new Secp256K1();
	secp->Init();

	InitGenratorTable();
}

// --------------------------------------------------------------------

void Rotor::InitGenratorTable()
{
	// Compute Generator table G[n] = (n+1)*G
	Point g = secp->G;

	Gn[0] = g;
	g = secp->DoubleDirect(g);
	Gn[1] = g;
	for (int i = 2; i < CPU_GRP_SIZE / 2; i++) {
		g = secp->AddDirect(g, secp->G);
		Gn[i] = g;
	}
	// _2Gn = CPU_GRP_SIZE*G
	_2Gn = secp->DoubleDirect(Gn[CPU_GRP_SIZE / 2 - 1]);

	char* ctimeBuff;
	time_t now = time(NULL);
	ctimeBuff = ctime(&now);
	printf("  Start Time   : %s", ctimeBuff);

	if (next > 0) {
		ifstream file777("giant_Continue.bat");
		string s777;
		string kogda;

		for (int i = 0; i < 5; i++) {
			getline(file777, s777);

			if (i == 1) {
				stroka = s777;
			}
			if (i == 3) {
				string kogda = s777;
				if (kogda != "") {
				}
			}
			if (i == 4) {
				string streek = s777;
				std::istringstream iss(streek);
				iss >> value777;
				uint64_t dobb = value777 / 1;
				rhex.Add(dobb);
			}
		}

		if (kogda == "") {
			ifstream file778("giant_START.bat");
			string s778;
			string kogda;
			stroka = "";
			for (int i = 0; i < 3; i++) {
				getline(file778, s778);

				if (i == 1) {
					stroka = s778;
				}
			}
		}
	}

	printf("start : %s (%d bit)\n", this->rangeStart.GetBase16().c_str(), this->rangeStart.GetBitLength());

	Int tThreads77;
	tThreads77.SetInt32(nbit2);
	rangeDiffcp.Div(&tThreads77);

	gir.Set(&rangeDiff2);
	Int reh;
	uint64_t nextt99;
	nextt99 = value777 * 1;
	reh.Add(nextt99);
	gir.Sub(&reh);
}

// --------------------------------------------------------------------

Rotor::~Rotor()
{
	delete secp;
	if (DATA)
	delete DATA;	//	free(DATA);
}

// --------------------------------------------------------------------

void Rotor::output2(std::string myoutputstr)
{

	FILE* f = stdout;
	bool needToClose = false;
	if (outputFile.length() > 0) {
		f = fopen(outputFile.c_str(), "a");
		if (f == NULL) {
			printf("  Cannot open %s for writing\n", outputFile.c_str());
			f = stdout;
		}
		else {
			needToClose = true;
		}
	}

	if (!needToClose)
		printf("\n");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	fprintf(f, "%s ", myoutputstr.c_str());
	if (needToClose)
		fclose(f);

}

void Rotor::output3(std::string myoutputstr)
{

	//	WaitForSingleObject(ghMutex, INFINITE);

	FILE* f = stdout;
	bool needToClose = false;
	if (outputFile.length() > 0) {
		f = fopen("myoutput.txt", "a");
		if (f == NULL) {
			printf("  Cannot open %s for writing\n", outputFile.c_str());
			f = stdout;
		}
		else {
			needToClose = true;
		}
	}

	if (!needToClose)
		printf("\n");
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	fprintf(f, "%s ", myoutputstr.c_str());
	//fprintf(f, "%s\n", pAddrHex.c_str());
	if (needToClose)
		fclose(f);
	//	ReleaseMutex(ghMutex);

}

// --------------------------------------------------------------------

DWORD WINAPI _FindKeyCPU(LPVOID lpParam)
{

	TH_PARAM* p = (TH_PARAM*)lpParam;
	//		printf(" pstart %s \n", p->rangeStart.GetBase16().c_str());
	//	printf(" pend %s \n\n", p->rangeEnd.GetBase16().c_str());

		p->obj->FindKeyCPU(p);

	return 0;
}

// ***   get rid of compressed ???
void Rotor::checkSingleXPoint(bool compressed, Int key, int i, Point p1,int callnum)
{
	unsigned char h0[32];

	// getting y point data might be able to unroll/ inline this 
	secp->GetXBytes(compressed, p1, h0); // does compressed do anything here???

	if (MatchXPoint((uint32_t*)h0)) {

		// this seems to be involved in the bug resulting in wronge offset numbers in results.
		Int k(&key);
		 k.Add((uint64_t)CPU_GRP_SIZE / 2); // this was a quick attempt to fix bug.  along with trying to subtract if negitive side.
		 if (callnum == 1)
		 {
			 k.Add((uint64_t)i+1);
		 }
		 else {
			 k.Sub((uint64_t)i+1);
		 }



#if DEBUG 1

		string s1 = p1.x.GetBase16().c_str(); // wont need after debugging done
		myoutputstr = myoutputstr + to_string(callnum) + " " + s1 + " ";



#endif // DEBUG = 1
		string s = p1.y.GetBase16().c_str();
		myoutputstr = myoutputstr + s + " " + k.GetBase16().c_str() + "\n";
#if DEBUG
#else
		endOfSearch = true;
#endif
	}
}

void Rotor::getCPUStartingKey(Int& tRangeStart, Int& tRangeEnd, Int& key, Point& startP)
{
	uint64_t nextt = 0;
	if (value777 > 1) {
		nextt = value777 / nbit2;
		tRangeStart.Add(nextt);
	}
	key.Set(&tRangeStart);
	Int kon;
	kon.Set(&tRangeStart);
	kon.Add(&rangeDiffcp);

	trk = trk + 1;
		printf("  loop (%d) : %s -> %s \n\n", trk, key.GetBase16().c_str(), rangeEnd.GetBase16().c_str());

	Int km(&key);
string mykm =	km.GetBase16().c_str();
	km.Add((uint64_t)CPU_GRP_SIZE / 2);

	startP = secp->ComputePublicKey(&km);
	string mytesty = startP.y.GetBase16().c_str();
string mytestx =	startP.x.GetBase16().c_str();
	printf("  loop (%d) : %s -> %s \n\n", trk, startP.x.GetBase16().c_str(), rangeEnd.GetBase16().c_str());

}

Point AddDirect1(Point& p1, Point& p2)
{
	Int _s;
	Int _p;
	Int dy;
	Int dx;
	Point r;
	r.z.SetInt32(1);
	dy.ModSub(&p2.y, &p1.y);
	dx.ModSub(&p2.x, &p1.x);
	dx.ModInv();
	_s.ModMulK1(&dy, &dx);    // s = (p2.y-p1.y)*inverse(p2.x-p1.x);
	_p.ModSquareK1(&_s);       // _p = pow2(s)
	r.x.ModSub(&_p, &p1.x);
	r.x.ModSub(&p2.x);       // rx = pow2(s) - p1.x - p2.x;
	r.y.ModSub(&p2.x, &r.x);
	r.y.ModMulK1(&_s);
	r.y.ModSub(&p2.y);       // ry = - p2.y - s*(ret.x-p2.x);
	return r;
}

void Rotor::FindKeyCPU(TH_PARAM* ph)
{
	// Global init
	int thId = ph->threadId;

	Int tRangeStart = ph->rangeStart;
	Int tRangeEnd = ph->rangeEnd;
	counters[thId] = 0;

	IntGroup* grp = new IntGroup(CPU_GRP_SIZE / 2 + 1);

	// Group Init
	Int key;// = new Int();
	Point startP;// = new Point();

		Point startP2;// = new Point();
		//fix this change to uknown - 139 -138 done

		//	startP2.x.SetBase16("ed01ff219ed5c1afc12d991a82e3063ddcee1fd53b46f7cad52a0d87a7112aed");
	startP2.x.SetBase16(myx1.c_str());
		//	startP2.y.SetBase16("73576e8a0991cdd5210a7a487e9bd4062cf52db06f3ad7b510f8b4656b93bacf"); //odd
 startP2.y.SetBase16(myy1.c_str()); //office

		startP2.z.SetInt32(1); // needed???
	getCPUStartingKey(tRangeStart, tRangeEnd, key, startP); // 
	//	string myx2 = startP.x.GetBase16().c_str();

	//	string myy2 = startP.y.GetBase16().c_str() ;
		Point startP3 = AddDirect1(startP, startP2);
		startP = startP3; // 

	Int* dx = new Int[CPU_GRP_SIZE / 2 + 1];
	Point* pts = new Point[CPU_GRP_SIZE];

	Int* dy = new Int();
	Int* dyn = new Int();
	Int* _s = new Int();
	Int* _p = new Int();
	Point* pp = new Point();
	Point* pn = new Point();
	grp->Set(dx);

	ph->hasStarted = true;
	//	ph->rKeyRequest = false;
	std::string myoutputstr2 = "";
	int mycounter = 1;
	while (!endOfSearch) {

		// Fill group
		int i;
		//	int hLength = (CPU_GRP_SIZE / 2 - 1);
		int hLength = (CPU_GRP_SIZE / 2)-1;

		for (i = 0; i < hLength; i++) 
		{	dx[i].ModSub(&Gn[i].x, &startP.x); }
		dx[i].ModSub(&Gn[i].x, &startP.x);  // For the first point
		dx[i + 1].ModSub(&_2Gn.x, &startP.x); // For the next center point

		// Grouped ModInv
		grp->ModInv();

		// We use the fact that P + i*G and P - i*G has the same deltax, so the same inverse
		// We compute key in the positive and negative way from the center of the group

		// center point
		pts[CPU_GRP_SIZE / 2] = startP;

		for (i = 0; i < hLength && !endOfSearch; i++) {

			*pp = startP;
			*pn = startP;

			// P = startP + i*G
			dy->ModSub(&Gn[i].y, &pp->y);

			_s->ModMulK1(dy, &dx[i]);       // s = (p2.y-p1.y)*inverse(p2.x-p1.x);
			_p->ModSquareK1(_s);            // _p = pow2(s)

			pp->x.ModNeg();
			pp->x.ModAdd(_p);
			pp->x.ModSub(&Gn[i].x);           // rx = pow2(s) - p1.x - p2.x;

			pp->y.ModSub(&Gn[i].x, &pp->x);
			pp->y.ModMulK1(_s);
			pp->y.ModSub(&Gn[i].y);           // ry = - p2.y - s*(ret.x-p2.x);

			checkSingleXPoint(true, key, i, *pp,1);
			dyn->Set(&Gn[i].y);
			dyn->ModNeg();
			dyn->ModSub(&pn->y);

			_s->ModMulK1(dyn, &dx[i]);      // s = (p2.y-p1.y)*inverse(p2.x-p1.x);
			_p->ModSquareK1(_s);            // _p = pow2(s)

			pn->x.ModNeg();
			pn->x.ModAdd(_p);

			pn->x.ModSub(&Gn[i].x);          // rx = pow2(s) - p1.x - p2.x;
			pn->y.ModSub(&Gn[i].x, &pn->x);
			pn->y.ModMulK1(_s);
			pn->y.ModAdd(&Gn[i].y);          // ry = - p2.y - s*(ret.x-p2.x);

			pts[CPU_GRP_SIZE / 2 + (i + 1)] = *pp;
			pts[CPU_GRP_SIZE / 2 - (i + 1)] = *pn;

			checkSingleXPoint(true, key, i, * pn, 2);
		}

		// First point (startP - (GRP_SZIE/2)*G)
		*pn = startP;
		dyn->Set(&Gn[i].y);
		dyn->ModNeg();
		dyn->ModSub(&pn->y);

		_s->ModMulK1(dyn, &dx[i]);
		_p->ModSquareK1(_s);

		pn->x.ModNeg();
		pn->x.ModAdd(_p);
		pn->x.ModSub(&Gn[i].x);

		pn->y.ModSub(&Gn[i].x, &pn->x);
		pn->y.ModMulK1(_s);
		pn->y.ModAdd(&Gn[i].y);

		pts[0] = *pn;

		// Next start point (startP + GRP_SIZE*G)
		*pp = startP;
		dy->ModSub(&_2Gn.y, &pp->y);

		_s->ModMulK1(dy, &dx[i + 1]);
		_p->ModSquareK1(_s);

		pp->x.ModNeg();
		pp->x.ModAdd(_p);
		pp->x.ModSub(&_2Gn.x);

		pp->y.ModSub(&_2Gn.x, &pp->x);
		pp->y.ModMulK1(_s);
		pp->y.ModSub(&_2Gn.y);
		startP = *pp;

	/*  not needed??? might not need some of above too
		for (int i = 0; i < CPU_GRP_SIZE && !endOfSearch; i++) {
		checkSingleXPoint(true, key, i, pts[i],3);
				}

*/

		key.Add((uint64_t)CPU_GRP_SIZE);
		counters[thId] += CPU_GRP_SIZE;
#if DEBUG 1
		mycounter++;
		if (mycounter > 4)
		{
			endOfSearch = true;
		}
#endif
		}
	ph->isRunning = false;

	delete grp;
	delete[] dx;
	delete[] pts;
	delete dy;
	delete dyn;
	delete _s;
	delete _p;
	delete pp;
	delete pn;
}

// --------------------------------------------------------------------

bool Rotor::isAlive(TH_PARAM* p)
{
	bool isAlive = true;
	int total = nbCPUThread;
	for (int i = 0; i < total; i++)
		isAlive = isAlive && p[i].isRunning;
	return isAlive;
}

// --------------------------------------------------------------------

bool Rotor::hasStarted(TH_PARAM* p)
{
	bool hasStarted = true;
	int total = nbCPUThread;
	for (int i = 0; i < total; i++)
		hasStarted = hasStarted && p[i].hasStarted;
	return hasStarted;
}

// --------------------------------------------------------------------

uint64_t Rotor::getCPUCount()
{
	uint64_t count = 0;
	for (int i = 0; i < nbCPUThread; i++)
		count += counters[i];
	return count;
}

// --------------------------------------------------------------------

void Rotor::SetupRanges(uint32_t totalThreads)
{
	Int threads;
	threads.SetInt32(totalThreads);
	rangeDiff.Set(&rangeEnd);
	rangeDiff.Sub(&rangeStart);
	rangeDiff.Div(&threads);
}

// --------------------------------------------------------------------

void Rotor::Search(int nbThread, bool& should_exit)
{
	double t0;
	double t1;
	endOfSearch = false;
	nbCPUThread = nbThread;

	nbFoundKey = 0;
	// setup ranges
	SetupRanges(1);
	memset(counters, 0, sizeof(counters));
	TH_PARAM* params = (TH_PARAM*)malloc((nbCPUThread) * sizeof(TH_PARAM));
	memset(params, 0, 0);
	// Launch CPU threads
		params[0].obj = this;
		params[0].threadId = 0;
		params[0].isRunning = true;
		params[0].rangeStart.Set(&rangeStart);
		rangeStart.Add(&rangeDiff);
		params[0].rangeEnd.Set(&rangeStart);

		_FindKeyCPU( (params + 0));

	uint64_t lastCount = 0;

	// Key rate smoothing filter
#define FILTER_SIZE 8
	double lastkeyRate[FILTER_SIZE];
	uint32_t filterPos = 0;

	double keyRate = 0.0;
	char timeStr[256];

	memset(lastkeyRate, 0, sizeof(lastkeyRate));

	Timer::Init();
	t0 = Timer::get_tick();
	startTime = t0;
//	Int p100;
	Int ICount;
//	p100.SetInt32(100);
	double completedPerc = 0;
	uint64_t rKeyCount = 0;
	while (isAlive(params)) {
		t0 = t1;
		if (should_exit || completedPerc > 100.5)
		{
			endOfSearch = true;
		}
	}
}

int32_t changeEndianness32(int32_t val)
{
	return (val << 24) |
		((val << 8) & 0x00ff0000) |
		((val >> 8) & 0x0000ff00) |
		((val >> 24) & 0x000000ff);
}
//------------------------------------------------------------------
bool Rotor::MatchXPoint(uint32_t* _h)
{

	uint32_t _mybyte = changeEndianness32(_h[7]);
#if DEBUG 1
	return true;
	int filter = 0x1;
	if (_mybyte % filter == 0)
#else
	int filter = 0x1000000;
	int filter2 = 0xFFFC2F;
	if (_mybyte % filter == 0 || _mybyte % filter == filter2)
#endif

	{ 	
		return true;
	}
	else {
		return false;
	}
}
