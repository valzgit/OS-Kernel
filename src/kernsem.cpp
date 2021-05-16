#include "kernsem.h"
#include "pcbqueue.h"
#include "schedule.h"
#include "pcb.h"
#include "sleeplst.h"


KernelSem::KernelSem(int init) {
	v = init;
	threads = new PCBQueue();
}

KernelSem::~KernelSem() {
	while (v < 0)
		signal();
}

int KernelSem::wait(Time maxTimeToWait) {
	if (--v >= 0) {
		PCB::running->ret = 1;
	} else {
		threads->add(PCB::running);
		PCB::running->state = PCB::Blocked;
		PCB::running->ksem = this;
		if (maxTimeToWait > 0) {
			PCB::sleepingThreads->add(PCB::running, maxTimeToWait);
		}
		dispatch();
	}
	return PCB::running->ret;
}

int KernelSem::signal(int n) {
	if (n < 0) {
		return n;
	}

	int startedZero = 0;
	if (n == 0) {
		n = 1;
		startedZero = 1;
	}

	int numSignalled = 0;

	for (int i = 0; i < n; i++) {
		 if (v++ < 0) {
			 PCB* pcb = threads->takeFirst();
			 if (pcb!=0) {
				 pcb->state = PCB::Ready;
				 Scheduler::put(pcb);
				 pcb->ret = 1;
				 pcb->ksem = 0;
				 PCB::sleepingThreads->remove(pcb);
				 numSignalled++;
			 }else
			 {ASSERT(pcb!=0);}
		 }
	}
//ISPITATI POZICIJU else IZNAD!
	if (startedZero!=0) {
		return 0;
	} else {
		return numSignalled;
	}
}

void KernelSem::signal(PCB* pcb) {
	threads->remove(pcb);
	v++;
}

