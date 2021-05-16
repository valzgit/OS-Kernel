#include "kernev.h"
#include "pcb.h"
#include "schedule.h"
#include "ivtentry.h"


KernelEv::KernelEv(IVTNo ivtNo) {
	myPCB = PCB::running;
	isBlocked = 0;
	isSignal = 0;
	this->ivtNo = ivtNo;


	IVTEntry* entry = IVTEntry::entries[ivtNo];
	if (entry!=0) {
		if (entry->ev == 0) {
			entry->ev = this;
		}
	}
}

KernelEv::~KernelEv() {

	IVTEntry* entry = IVTEntry::entries[ivtNo];
	if (entry!=0) {
		if (entry->ev == this) {
			entry->ev = 0;
		}
	}

	if (isBlocked)
		signal();
}

void KernelEv::wait() {
	if (PCB::running != myPCB)
		return;

	if (isSignal != 0) {
		isSignal = 0;
	}
	else {
		isBlocked = 1;
		PCB::running->state = PCB::Blocked;
		dispatch();
	}
}

void KernelEv::signal() {
	if (isBlocked != 0) {
		isBlocked = 0;
		myPCB->state = PCB::Ready;
		Scheduler::put(myPCB);
	}
	else {
		isSignal = 1;
	}
}


