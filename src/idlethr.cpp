#include "idlethr.h"
//njegov ID=2

IdleThread::IdleThread() : Thread(400, 1), runFlag(1) {}


IdleThread::~IdleThread() {
	waitToComplete();
}

void IdleThread::run() {
	while (runFlag!=0) {
		dispatch();
	}
}



