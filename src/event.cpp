#include "event.h"
#include "kernev.h"
#include "pcb.h"


Event::Event(IVTNo ivtNo) {
	LOCK
	myImpl = new KernelEv(ivtNo);
	UNLOCK
}

Event::~Event() {
	LOCK
	delete myImpl;
	UNLOCK
}

void Event::wait() {
	LOCK
	myImpl->wait();
	UNLOCK
}

void Event::signal() {
	LOCK
	myImpl->signal();
	UNLOCK
}



