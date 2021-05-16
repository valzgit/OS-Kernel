#ifndef H_SLEEPLST_H
#define H_SLEEPLST_H

#include "thread.h"

class SleepList {
public:
	SleepList();
	 ~SleepList();

	 void add(PCB* pcb, Time timeToSleep);
	 void updateSleepingThreads();
	 void remove(PCB* pcb);

	struct Elem {
		PCB* pcb;
		Time timeToSleep;
		Elem* next;
		Elem(PCB* pcb, Time timeToSleep) {
			this->pcb = pcb;
			this->timeToSleep = timeToSleep;
			next = 0;
		}
	};
	Elem* head;
};

#endif /* H_SLEEPLST_H */
