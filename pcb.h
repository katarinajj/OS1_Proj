#ifndef PCB_H_
#define PCB_H_

#include "asystem.h"
#include "list.h"
#include "thread.h"
#include "SCHEDULE.h"

const StackSize maxStackSize = 65536;

enum State {INITIALIZED, READY, SUSPENDED, TERMINATED};

void idleBody();

class PCB {
public:
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned *stack;

	unsigned timeSlice;
	State state;

	Thread *myThread;
	List *waitingForThis;

	unsigned unblockedByTime;

	void start();
	void waitToComplete();
	~PCB();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	PCB (StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)() = PCB::wrapper);
	static void wrapper();
	PCB();

	ID id;

private:
	static ID staticID;


};


#endif /* PCB_H_ */
