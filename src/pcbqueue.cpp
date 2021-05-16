#include "pcbqueue.h"
#include "pcb.h"


PCBQueue::PCBQueue() {
	head = tail = 0;
}

PCBQueue::~PCBQueue() {
	while (takeFirst() != 0);
}

void PCBQueue::add(PCB* pcb) {
	Elem* elem = new Elem(pcb);
	if (head == 0)
		head = elem;
	else
		tail->next = elem;
	tail = elem;
}

PCB* PCBQueue::takeFirst() {
	if (head == 0)
		return 0;

	Elem* elem = head;
	head = head->next;
	if (head == 0)
		tail = 0;
	PCB* pcb = elem->pcb;
	elem->next=0;
	elem->pcb=0;
	delete elem;
	return pcb;
}//dobar

void PCBQueue::remove(PCB* pcb) {
	Elem* cur = head;
	Elem* prev = 0;
	while ((cur!=0) && ((cur->pcb) != pcb)) {
		prev = cur;
		cur = cur->next;
	}

	if (cur!=0) {
		if (prev!=0)
			prev->next = cur->next;
		else
			head = cur->next;

		if (cur == tail) {
			tail = prev;
		}
        cur->pcb=0;
        cur->next=0;
		delete cur;
	}
}

PCB* PCBQueue::findPCB(ID id) {
	Elem* elem = head;
	while ((elem!=0) && (((elem->pcb)->id) != id)) {
		elem = elem->next;
	}

	if (elem!=0) {
		return elem->pcb;
	}
	else {
		return 0;
	}
}

