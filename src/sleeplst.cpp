#include "sleeplst.h"
#include "pcb.h"
#include "schedule.h"
#include "kernsem.h"


SleepList::SleepList() {
	head = 0;
}
//dobar

SleepList::~SleepList() {
	while (head!=0) {
		PCB* pcb = head->pcb;
		pcb->state = PCB::Ready;
		Scheduler::put(pcb);
		pcb->ret = 0;
		Elem* old = head;
		head = head->next;
		old->pcb=0;
		old->next=0;
		delete old;
	}
}
//dobar

void SleepList::add(PCB* pcb, Time timeToSleep) {
	if ((timeToSleep <= 0) || (pcb == 0)) {
		return;
	}

	Elem* elem = new Elem(pcb, timeToSleep);

	Elem* cur = head;
	Elem* prev = 0;

	while (cur != 0) {
		if (cur->timeToSleep <= elem->timeToSleep) {
			elem->timeToSleep -= cur->timeToSleep;
			prev = cur;
			cur = cur->next;
		}
		else {
			cur->timeToSleep -= elem->timeToSleep;
			break;
		}
	}

	if (prev!=0)
		prev->next = elem;
	else
		head = elem;

	elem->next = cur;
}
//trebalo bi da je dobro
void SleepList::updateSleepingThreads() {
	if (head != 0)
		head->timeToSleep--;
	else return;

	while ((head != 0) && (head->timeToSleep == 0)) {
		Elem* old = head;
		head = head->next;
		PCB* pcb = old->pcb;
		pcb->state = PCB::Ready;
		Scheduler::put(pcb);
		ASSERT(pcb->ksem != 0);
		pcb->ksem->signal(pcb);
		pcb->ksem = 0;
		pcb->ret = 0;
        old->pcb=0;
        old->next=0;
		delete old;
	}
}
//dobro

void SleepList::remove(PCB* pcb) {
	if ((pcb == 0) || (head == 0)) {
		return;
	}

	Elem* cur = head;
	Elem* prev = 0;
	while ((cur!=0) && ((cur->pcb) != pcb)) {
		prev = cur;
		cur = cur->next;
	}

	if (cur!=0) {
		if (prev!=0) {
			prev->next = cur->next;
		} else {
			head = cur->next;
		}

		if ((cur->next)!=0) {
			((cur->next)->timeToSleep) += (cur->timeToSleep);
		}
        cur->next=0;
        cur->pcb=0;
		delete cur;
	}
}
//dobro
