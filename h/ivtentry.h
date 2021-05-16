#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "kernev.h"
#include "thread.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);
class KernelEv;

#define PREPAREENTRY(num, callPrev) \
void interrupt intr##num(...); \
IVTEntry entry##num(num, &intr##num); \
void interrupt intr##num(...) { \
	if (callPrev!=0) { \
		pInterrupt prevIntr = entry##num.prevIntr; \
		if (prevIntr!=0) { \
			(*prevIntr)(); \
		} \
	} \
	if (entry##num.ev!=0) { \
		entry##num.ev->signal(); \
	} \
	dispatch(); \
}


class IVTEntry {
public:
	IVTEntry(IVTNo num, pInterrupt newIntr);
	~IVTEntry();

	pInterrupt prevIntr;
	IVTNo num;
	KernelEv* ev; // dogadjaj registrovan da mu se poziva signal() kad dodje prekid
	static IVTEntry* entries[256];
};


#endif /* H_IVTENTRY_H_ */
