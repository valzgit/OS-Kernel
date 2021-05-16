#ifndef H_PCBQUEUE_H_
#define H_PCBQUEUE_H_

typedef int ID;
class PCB;

class PCBQueue {
public:
	PCBQueue();

	~PCBQueue();

	void add(PCB* pcb);
	void remove(PCB* pcb);
	PCB* takeFirst();
	PCB* findPCB(ID id);

	struct Elem {
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb) {
			this->pcb = pcb;
			next = 0;
		}
	};
private:
	Elem* head;
	Elem* tail;
};


#endif /* H_PCBQUEUE_H_ */
