#include "pcb.h"
#include "schedule.h"
#include <iostream.h>
#include "pcbqueue.h"
#include "sleeplst.h"
#include "idlethr.h"


#include <stdlib.h>
#ifndef BCC_BLOCK_IGNORE
void _FAR * _FARFUNC operator new(unsigned size) {
	asm { pushf; cli; }
	void _FAR * membuf = malloc(size);
	asm { popf; }
	return membuf;
}

void _FARFUNC operator delete(void _FAR * p) {
	asm { pushf; cli; }
	free(p);
	asm { popf; }
}
#endif

int lockFlag=0;
int dispIgnored=0;
void lockF(){lockFlag=1;}
void unlockF(){lockFlag=0;if(dispIgnored==1){dispIgnored=0;dispatch();}}

PCB* PCB::running = 0;
Time PCB::runCounter = 0;
pInterrupt PCB::prevTimer = 0;
SleepList* PCB::sleepingThreads = 0;
IdleThread* PCB::idleThread = 0;
PCB* PCB::idlePCB = 0;
int PCB::creatingIdleThread = 0;

int PCB::Blocked = 1;
int PCB::Ready = 2;
int PCB::Finished = 3;
int PCB::New = 4;

ID PCB::nextId = 0;

PCBQueue* PCB::allThreads = 0;

void setVect(IVTNo ivtNo, pInterrupt newIntr) {
	LOCK
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, newIntr);
#endif
	UNLOCK
}

pInterrupt getVect(IVTNo ivtNo) {
	LOCK
	pInterrupt retVal = 0;
#ifndef BCC_BLOCK_IGNORE
	retVal = getvect(ivtNo);
#endif
	UNLOCK
	return retVal;
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread) {
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	myThread = thread;
	ss = sp = bp = 0;
	stack = 0;
	state = PCB::New;
	blockedThreads = new PCBQueue();
	if (creatingIdleThread != 0)
		idlePCB = this;
	id = ++nextId;
	allThreads->add(this);
	ksem = 0;ret=0;
}

PCB::~PCB() {
	allThreads->remove(this);
	delete blockedThreads;
	delete[] stack;
}

void PCB::setupStack() {
	StackSize size = stackSize / sizeof(unsigned int);
	stack = new unsigned int[size];
	stack[size - 1] = SEGM(myThread);
	stack[size - 2] = OFFS(myThread);
	stack[size - 3] = 0; // ret_wrapper CS
	stack[size - 4] = 0; // ret_wrapper IP
	stack[size - 5] = 0x200;
	stack[size - 6] = SEGM(&wrapper);
	stack[size - 7] = OFFS(&wrapper);

	ss = SEGM(stack + size - 16);
	sp = OFFS(stack + size - 16); //sizeof(tip)
	bp = OFFS(stack + size - 16);
}

void PCB::start() {
	if (state == PCB::New) {
		setupStack();
		if (this != PCB::idlePCB)
			Scheduler::put(this);
		state = PCB::Ready;
	}
}

ID PCB::getId() {
	return id;
}

Thread* PCB::getThreadById(ID id) {
	PCB* pcb=0;
	pcb = allThreads->findPCB(id);
	if (pcb!=0) {
		return pcb->myThread;
	}
	else {
		return 0;
	}
}

void interrupt PCB::dispatchIntr() {
	static unsigned int tss, tsp, tbp;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov tss, ss;
		mov tsp, sp;
		mov tbp, bp;
	}
#endif
	running->ss = tss;
	running->sp = tsp;
	running->bp = tbp;

	if (running != PCB::idlePCB && running->state == PCB::Ready){
		Scheduler::put(running);}
	running = Scheduler::get();
	if (running == 0) {
		running = PCB::idlePCB;
	}
	ASSERT(running != 0);

	tss = running->ss;
	tsp = running->sp;
	tbp = running->bp;
#ifndef BCC_BLOCK_IGNORE
	asm {
		mov ss, tss;
		mov sp, tsp;
		mov bp, tbp;
	}
#endif
	runCounter = 0;
}

void PCB::wrapper(Thread* thread) {
	thread->run();

	LOCK
	PCB* pcb=0;
	while ((pcb=running->blockedThreads->takeFirst())!=0) {
		pcb->state = PCB::Ready;
		Scheduler::put(pcb);
	}

	running->state = PCB::Finished;
	UNLOCK
	dispatch();
}

void PCB::waitToComplete() {
	if ((running != this) && (this->state == PCB::Ready || this->state == PCB::Blocked))
	{
		running->state = PCB::Blocked;
		this->blockedThreads->add(running);
		dispatch();
	}
}

void tick();

void interrupt PCB::timerIntr(...) {
	(*prevTimer)();
	runCounter++;
	tick();

	sleepingThreads->updateSleepingThreads();
    if(lockFlag==1){dispIgnored=1;return;}
	if (runCounter >= running->timeSlice && running->timeSlice > 0) {
		dispatch();
	}
}

