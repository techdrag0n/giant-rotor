#include "IntGroup.h"

using namespace std;

IntGroup::IntGroup(int size) {
	this->size = size;
	subp = (Int*)malloc(size * sizeof(Int));
}

IntGroup::~IntGroup() {
	free(subp);
}

void IntGroup::Set(Int* pts) {
	ints = pts;
}

// Compute modular inversion of the whole group
void IntGroup::ModInv() {

	Int newValue;
	Int inverse;

	subp[0].Set(&ints[0]);
	for (int i = 1; i < size; i++) {
		subp[i].ModMulK1(&subp[i - 1], &ints[i]);

//	printf(" subp[%s] : %s \n\n",to_string(i).c_str(), subp[i].GetBase16().c_str());
	}	// Do the inversion
	inverse.Set(&subp[size - 1]);
	inverse.ModInv();

//	printf(" modinv : %s \n\n",  inverse.GetBase16().c_str());

	for (int i = size - 1; i > 0; i--) {
		newValue.ModMulK1(&subp[i - 1], &inverse);
		inverse.ModMulK1(&ints[i]);
		ints[i].Set(&newValue);
	}

	ints[0].Set(&inverse);
}