#ifndef H_KERNEV_H_
#define H_KERNEV_H_

typedef unsigned char IVTNo;

class PCB;

class KernelEv {
public:
	KernelEv(IVTNo ivtNo);
	 ~KernelEv();
	 void wait();
	 void signal();

	PCB* myPCB;
	IVTNo ivtNo;

	int isBlocked;
	int isSignal;

};


#endif /* H_KERNEV_H_ */
