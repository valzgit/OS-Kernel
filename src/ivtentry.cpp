#include "ivtentry.h"
#include "pcb.h"

IVTEntry* IVTEntry::entries[256] = {0};

IVTEntry::IVTEntry(IVTNo num, pInterrupt newIntr) {
	LOCK
	entries[num] = this;
	ev = 0;
	this->num = num;
	prevIntr = getVect(num);
	setVect(num, newIntr);
	UNLOCK
}

IVTEntry::~IVTEntry() {
	LOCK
	entries[num] = 0;
	setVect(num, prevIntr);
	if (prevIntr) {
		(*prevIntr)();
	}
	UNLOCK
}

