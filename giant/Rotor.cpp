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

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

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

// ----------------------------------------------------------------------------

Rotor::~Rotor()
{
	delete secp;
	if (DATA)
	delete DATA;	//	free(DATA);

}

// ----------------------------------------------------------------------------
/*
void Rotor::output(std::string pAddrHex, std::string pubKey)
{
 myoutputstr = myoutputstr + pubKey.c_str() + " " + pAddrHex.c_str() + "\n";

}
*/
void Rotor::output2(std::string myoutputstr)
{

	//	WaitForSingleObject(ghMutex, INFINITE);

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
	//fprintf(f, "%s\n", pAddrHex.c_str());
	if (needToClose)
		fclose(f);
	//	ReleaseMutex(ghMutex);

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

// ----------------------------------------------------------------------------

DWORD WINAPI _FindKeyCPU(LPVOID lpParam)
{

	TH_PARAM* p = (TH_PARAM*)lpParam;
	//		printf(" pstart %s \n", p->rangeStart.GetBase16().c_str());
	//	printf(" pend %s \n\n", p->rangeEnd.GetBase16().c_str());

		p->obj->FindKeyCPU(p);

	return 0;
}


// ***   get rid of compressed ???
void Rotor::checkSingleXPoint(bool compressed, Int key, int i, Point p1)
{
	unsigned char h0[32];

	// getting y point data might be able to unroll/ inline this 
	secp->GetXBytes(compressed, p1, h0);

	if (MatchXPoint((uint32_t*)h0)) {

		// this seems to be involved in the bug resulting in wronge offset numbers in results.

		Int k(&key);
		 k.Add((uint64_t)CPU_GRP_SIZE / 2 + 1); // this was a quick attempt to fix bug.  along with trying to subtract if negitive side.
	//		k.Add((uint64_t)CPU_GRP_SIZE / 2+1 + i);  // half
		k.Add((uint64_t)i);

		string s = p1.x.GetBase16().c_str();
	//	std::ostringstream oss;
	//	oss << std::right << std::setfill('0') << std::setw(64) << s;
		myoutputstr = myoutputstr + s + " ";

		s = p1.y.GetBase16().c_str();
	//	std::ostringstream oss2;
	//	oss2 << std::right << std::setfill('0') << std::setw(64) << s;

		myoutputstr = myoutputstr + s + " " + k.GetBase16().c_str() + "\n";

		endOfSearch = true;
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
//	Point startP2;// = new Point();
	// set startP  to unknown point
	// 0x1000 = 04       175E159F728B865A72F99CC6C6FC846DE0B93833FD2222ED73FCE5B551E5B739D3506E0D9E3C79EBA4EF97A51FF71F5EACB5955ADD24345C6EFA6FFEE9FED695
	// 0x0100000000 = 04 100F44DA696E71672791D0A09B7BDE459F1215A29B3C03BFEFD7835B39A48DB0CDD9E13192A00B772EC8F3300C090666B7FF4A18FF5195AC0FBD5CD62BC65A09
	//fix this change to uknown - 119 -118 done

//	startP2.x.SetBase16("ed01ff219ed5c1afc12d991a82e3063ddcee1fd53b46f7cad52a0d87a7112aed");

//	startP2.x.SetBase16(myx1.c_str());

//	startP2.y.SetBase16("73576e8a0991cdd5210a7a487e9bd4062cf52db06f3ad7b510f8b4656b93bacf"); //odd

	// 73576e8a0991cdd5210a7a487e9bd4062cf52db06f3ad7b510f8b4656b93bacf
	// 8ca89175f66e322adef585b781642bf9d30ad24f90c5284aef074b99946c4160
	//	startP2.y.SetBase16("6f430175e964aa005535f9c98d7141c1f4304d0e353b8439f242ec096b8d3522"); //office
// startP2.y.SetBase16(myy1.c_str()); //office

//	startP2.z.SetInt32(1);
	getCPUStartingKey(tRangeStart, tRangeEnd, key, startP); // 
//	string myx2 = startP.x.GetBase16().c_str();

//	string myy2 = startP.y.GetBase16().c_str() ;

//	Point startP3 = AddDirect1(startP, startP2);
//	startP = startP3; // is this the problem????

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
	while (!endOfSearch) {

		// Fill group
		int i;
		int hLength = (CPU_GRP_SIZE / 2 - 1);
		//	int hLength = (CPU_GRP_SIZE / 2);

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
		//printf("h= %i \n", hLength);




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

			checkSingleXPoint(true, key, 0-i, *pp);
			dyn->Set(&Gn[i].y);
			dyn->ModNeg();
			dyn->ModSub(&pn->y);

			_s->ModMulK1(dyn, &dx[i]);      // s = (p2.y-p1.y)*inverse(p2.x-p1.x);
			_p->ModSquareK1(_s);            // _p = pow2(s)

			pn->x.ModNeg();
			pn->x.ModAdd(_p);

			// output pn->x

//			printf("  p.x1 : %s\n", pn->x.GetBase16().c_str());


			pn->x.ModSub(&Gn[i].x);          // rx = pow2(s) - p1.x - p2.x;

// ewq	
			pn->y.ModSub(&Gn[i].x, &pn->x);
			pn->y.ModMulK1(_s);
			pn->y.ModAdd(&Gn[i].y);          // ry = - p2.y - s*(ret.x-p2.x);

			pts[CPU_GRP_SIZE / 2 + (i + 1)] = *pp;
			pts[CPU_GRP_SIZE / 2 - (i + 1)] = *pn;

			checkSingleXPoint(true, key, 0-i, *pn);

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

		 // ....
				for (int i = 0; i < CPU_GRP_SIZE && !endOfSearch; i++) {
					int myi = 0 - i;
					checkSingleXPoint(true, key, myi, pts[i]);
				//			checkSingleXPoint(true, key, i, pts[i]);
				}

		key.Add((uint64_t)CPU_GRP_SIZE);
		counters[thId] += CPU_GRP_SIZE; // Point
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

// ----------------------------------------------------------------------------

bool Rotor::isAlive(TH_PARAM* p)
{
	bool isAlive = true;
	int total = nbCPUThread;
	for (int i = 0; i < total; i++)
		isAlive = isAlive && p[i].isRunning;
	return isAlive;
}

// ----------------------------------------------------------------------------

bool Rotor::hasStarted(TH_PARAM* p)
{

	bool hasStarted = true;
	int total = nbCPUThread;
	for (int i = 0; i < total; i++)
		hasStarted = hasStarted && p[i].hasStarted;
	return hasStarted;
}

// ----------------------------------------------------------------------------

uint64_t Rotor::getCPUCount()
{

	uint64_t count = 0;
	for (int i = 0; i < nbCPUThread; i++)
		count += counters[i];
	return count;

}

// ----------------------------------------------------------------------------

void Rotor::SetupRanges(uint32_t totalThreads)
{
	Int threads;
	threads.SetInt32(totalThreads);
	rangeDiff.Set(&rangeEnd);
	rangeDiff.Sub(&rangeStart);
	rangeDiff.Div(&threads);
}

// ----------------------------------------------------------------------------

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
//	printf("\n");
	TH_PARAM* params = (TH_PARAM*)malloc((nbCPUThread) * sizeof(TH_PARAM));
	memset(params, 0, 0);
	// Launch CPU threads
//	for (int i = 0; i < nbCPUThread; i++) {
		params[0].obj = this;
		params[0].threadId = 0;
		params[0].isRunning = true;
		params[0].rangeStart.Set(&rangeStart);
		rangeStart.Add(&rangeDiff);
		params[0].rangeEnd.Set(&rangeStart);
	//	DWORD thread_id;
	//	CreateThread(NULL, 0, _FindKeyCPU, (void*)(params + i), 0, &thread_id);

		_FindKeyCPU( (params + 0));


	//	ghMutex = CreateMutex(NULL, FALSE, NULL);
//	}

//	printf("\n");
	uint64_t lastCount = 0;
//	uint64_t gpuCount = 0;//remove

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

	//	uint64_t count = getCPUCount();
		/*	ICount.SetInt64(count);
		int completedBits = ICount.GetBitLength();

		completedPerc = CalcPercantage(ICount, rangeStart, rangeDiff2);

		minuty++;

		t1 = Timer::get_tick();
		keyRate = (double)(count - lastCount) / (t1 - t0);
		lastkeyRate[filterPos % FILTER_SIZE] = keyRate;
		filterPos++;

		// KeyRate smoothing
		double avgKeyRate = 0.0;
		uint32_t nbSample;
		for (nbSample = 0; (nbSample < FILTER_SIZE) && (nbSample < filterPos); nbSample++) {
			avgKeyRate += lastkeyRate[nbSample];
		}
		avgKeyRate /= (double)(nbSample);

		zhdat++;

		unsigned long long int years88, days88, hours88, minutes88, seconds88;
		double avgKeyRate2 = avgKeyRate * 1;
		rhex.Add(avgKeyRate2);
		double avgKeyRate5 = avgKeyRate * 1;
		unsigned long long int input88;
		unsigned long long int input55;
		unsigned long long int minnn;
		string streek77 = rangeDiffbar.GetBase10().c_str();
		std::istringstream iss(streek77);
		iss >> input55;
		minnn = input55 - count;
		input88 = minnn / avgKeyRate5;
		years88 = input88 / 60 / 60 / 24 / 365;
		days88 = (input88 / 60 / 60 / 24) % 365;
		hours88 = (input88 / 60 / 60) % 24;
		minutes88 = (input88 / 60) % 60;
		seconds88 = input88 % 60;
	
		if (years88 > 0) {
			if (isAlive(params)) {
				memset(timeStr, '\0', 256);
				printf("\r  [%s] [F: %d] [Y:%03llu D:%03llu] [C: %lf %%] [CPU %d: %.2f Mk/s] [T: %s]  ",
					toTimeStr(t1, timeStr),
					nbFoundKey,
					years88,
					days88,
					completedPerc,
					nbit2,
					avgKeyRate / 1000000.0,
					formatThousands(count).c_str());
			}
		}
		else {

			if (isAlive(params)) {
				memset(timeStr, '\0', 256);
				printf("\r  [%s] [F: %d] [D:%03llu %02llu:%02llu:%02llu] [C: %lf %%] [CPU %d: %.2f Mk/s] [T: %s]   ",
					toTimeStr(t1, timeStr),
					nbFoundKey,
					days88,
					hours88,
					minutes88,
					seconds88,
					completedPerc,
					nbit2,
					avgKeyRate / 1000000.0,
					formatThousands(count).c_str());
			}

		}
*/
//		lastCount = count;
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

	if (_mybyte % 0x10 == 0) { // will be replacing with filter & filter2
	
//	if (_h[7] < 0x1000000) {
		return true;
	}
	else {
		return false;
	}
}
// ----------------------------------------------------
/*
std::string Rotor::formatThousands(uint64_t x)
{
	char buf[32] = "";
	sprintf(buf, "%llu", x);
	std::string s(buf);
	int len = (int)s.length();
	int numCommas = (len - 1) / 3;
	if (numCommas == 0) {
		return s;
	}

	std::string result = "";
	int count = ((len % 3) == 0) ? 0 : (3 - (len % 3));
	for (int i = 0; i < len; i++) {
		result += s[i];

		if (count++ == 2 && i < len - 1) {
			result += ",";
			count = 0;
		}
	}
	return result;
}


// ----------------------------------------------------------------------------

char* Rotor::toTimeStr(int sec, char* timeStr)
{
	int h, m, s;
	h = (sec / 3600);
	m = (sec - (3600 * h)) / 60;
	s = (sec - (3600 * h) - (m * 60));
	sprintf(timeStr, "%0*d:%0*d:%0*d", 2, h, 2, m, 2, s);
	return (char*)timeStr;
}
*/