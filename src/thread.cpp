//sredio getRunningId i prestao da puca!?!?!
#include "pcb.h"

Thread::Thread(StackSize stackSize, Time timeSlice) {
	LOCK
	myPCB = new PCB(stackSize, timeSlice, this);
	UNLOCK
}

Thread::~Thread() {
	waitToComplete();
	LOCK
	delete myPCB;
	UNLOCK
}

void Thread::start() {
	LOCK
	myPCB->start();
	UNLOCK
}

void Thread::waitToComplete() {
	LOCK
	myPCB->waitToComplete();
	UNLOCK
}

void dispatch() {
	LOCK
	PCB::dispatchIntr();
	UNLOCK
}


ID Thread::getId() {
	LOCK
	ID id = myPCB->getId();
	UNLOCK
	return id;
}

ID Thread::getRunningId() {
	LOCK
	ID id =0;
	if(PCB::running!=0)
	id = PCB::running->getId();
	UNLOCK
	return id;
}

Thread* Thread::getThreadById(ID id) {
	LOCK
	Thread* t = PCB::getThreadById(id);
	UNLOCK
	return t;
}

