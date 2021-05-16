#include "semaphor.h"
#include "kernsem.h"
#include "pcb.h"


Semaphore::Semaphore(int init) {
	LOCK
	myImpl = new KernelSem(init);
	UNLOCK
}

Semaphore::~Semaphore() {
	LOCK
	delete myImpl;
	UNLOCK
}

int Semaphore::wait(Time maxTimeToWait) {
	LOCK
	int ret = myImpl->wait(maxTimeToWait);
	UNLOCK
	return ret;
}

int Semaphore::signal(int n) {
	LOCK
	int ret = myImpl->signal(n);
	UNLOCK
	return ret;
}

int Semaphore::val() const {
	LOCK
	int v = myImpl->v;
	UNLOCK
	return v;
}



