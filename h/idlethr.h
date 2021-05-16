
#ifndef H_IDLETHR_H_
#define H_IDLETHR_H_
#include "thread.h"

class IdleThread : public Thread {
public:
	IdleThread();
	virtual ~IdleThread();
	void run();

	int runFlag;
};


#endif /* H_IDLETHR_H_ */
