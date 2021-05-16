#include "pcb.h"
#include "sleeplst.h"
#include "idlethr.h"
#include "pcbqueue.h"

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	PCB::allThreads = new PCBQueue();
	PCB::sleepingThreads = new SleepList();
	Thread* runningThread = new Thread(0);
	PCB::running = runningThread->myPCB;
	PCB::running->state = PCB::Ready;

	PCB::creatingIdleThread = 1;
	PCB::idleThread = new IdleThread();
	PCB::creatingIdleThread = 0;
	PCB::idleThread->start();

	PCB::prevTimer = getVect(8);
	setVect(8, &PCB::timerIntr);

	int result = userMain(argc, argv);

	delete PCB::sleepingThreads;
	setVect(8, PCB::prevTimer);

	PCB::idleThread->runFlag = 0;
	delete PCB::idleThread;
	PCB::running->state = PCB::Finished;
	PCB::running = 0;
	delete runningThread;
	delete PCB::allThreads;

	return result;
}
