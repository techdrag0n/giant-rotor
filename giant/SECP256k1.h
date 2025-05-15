#ifndef SECP256K1H
#define SECP256K1H

#include "Point.h"
#include <string>
#include <vector>

class Secp256K1
{

public:

	Secp256K1();
	~Secp256K1();
	void Init();
	Point ComputePublicKey(Int* privKey);
//	Point NextKey(Point& key);
//	void Check();
//	bool  EC(Point& p);
	void GetXBytes(bool compressed, Point& pubKey, unsigned char* publicKeyBytes);

//	std::string GetPublicKeyHex(bool compressed, Point& pubKey);

//	Point Add(Point& p1, Point& p2);
	Point Add2(Point& p1, Point& p2);
	Point AddDirect(Point& p1, Point& p2);
//	Point Double(Point& p);
	Point DoubleDirect(Point& p);

	Point G;                 // Generator
	Int   order;             // Curve order
	Point GTable[256 * 32];     // Generator table
private:

	uint8_t GetByte(std::string& str, int idx);

//	Int GetY(Int x, bool isEven);


};

#endif // SECP256K1H
