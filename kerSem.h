
// imam globalnu

#include "semaphor.h"
#include "list.h"
#include "timeList.h"

List* allKerSems;

class KernelSem {
public:
	Semaphore *mySem;
	List *blockedPCBs;
	volatile timeList *waitingPCBs;
	
	KernelSem (int init, Semaphore *mySem = 0);
	~KernelSem ();
	
	int wait (Time maxTimeToWait);
	void signal();
	
	int val () const; // Returns the current value of the semaphore

	void block();
	void unblock();

private:
	int value;
	
}