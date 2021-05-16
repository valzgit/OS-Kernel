
#ifndef H_KERNSEM_H_
#define H_KERNSEM_H_

class PCB;
class PCBQueue;
typedef unsigned int Time;

class KernelSem {
public:
	KernelSem(int init=1);
	 ~KernelSem();
	 int wait(Time maxTimeToWait);
	 int signal(int n=0);
	 void signal(PCB* pcb);
	int v;
	PCBQueue* threads;
};


#endif /* H_KERNSEM_H_ */
