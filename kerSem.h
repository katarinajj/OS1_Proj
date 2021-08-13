#ifndef KERSEM_H_
#define KERSEM_H_

#include "listTime.h"
#include "semaphor.h"
#include "pcb.h"

class KernelSem {
public:
	Semaphore *mySem;

	List *blockedPCBs;
	//volatile TimeList *waitingPCBs;
	TimeList *waitingPCBs;

	KernelSem(int init, Semaphore *mySem);
	~KernelSem();

	int wait(Time maxTimeToWait);
	void signal();

	int val() const;

	int block();
	int blockTime(Time maxTimeToWait);
	void unblock();

	friend class TimeList;
private:
	int value;
};



#endif /* KERSEM_H_ */
