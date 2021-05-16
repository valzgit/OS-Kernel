#ifndef H_PCB_H_
#define H_PCB_H_

#include "thread.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

void setVect(IVTNo ivtNo, pInterrupt newIntr);
pInterrupt getVect(IVTNo ivtNo);

#ifndef BCC_BLOCK_IGNORE
#include <dos.h>
#include <assert.h>

#define SEGM(val) FP_SEG(val)
#define OFFS(val) FP_OFF(val)

#define ASSERT(val) assert(val)
#else
#define SEGM(val) 0
#define OFFS(val) 0

#define ASSERT(val)
#endif


#ifndef BCC_BLOCK_IGNORE
#define LOCK asm { pushf; cli }
#define UNLOCK asm { popf }
#else

#define LOCK
#define UNLOCK
#endif

class PCBQueue;
class SleepList;
class IdleThread;
class KernelSem;

void lockF();
void unlockF();
class PCB {
public:
	PCB(StackSize stackSize, Time timeSlice, Thread* thread);
	~PCB();

	void setupStack();
	void start();

	static void interrupt dispatchIntr();
	static void wrapper(Thread* thread);

	void waitToComplete();

	ID getId();
	static Thread* getThreadById(ID id);

	static void interrupt timerIntr(...);
	static Time runCounter; // koliko dugo running nit radi uzastopno
	static pInterrupt prevTimer;
	static SleepList* sleepingThreads;

	static PCB* running;

	StackSize stackSize;
	Time timeSlice;
	Thread* myThread;
	unsigned int ss, sp, bp;
	unsigned int* stack;

	int state;
	static int Blocked;
	static int Ready;
	static int Finished;
	static int New;


	PCBQueue* blockedThreads;

	static IdleThread* idleThread;
	static PCB* idlePCB;
	static int creatingIdleThread;

	static ID nextId;
	ID id;

	static PCBQueue* allThreads;

	KernelSem* ksem;
	int ret;
};


#endif /* H_PCB_H_ */
