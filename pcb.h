#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "SCHEDULE.h"
#include "list.h"

const StackSize maxStackSize = 65536;

enum State {INITIALIZED, READY, RUNNING, SUSPENDED, TERMINATED};


class PCB {
public:
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned *stack;

	unsigned timeSlice;
	State state;

	Thread *myThread;
	//List waitingForThis;

	static volatile PCB* running;
	static PCB* idleThread;

	static List *allPCBs;

	void start();
	void waitToComplete();
	~PCB();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	PCB (StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)() = PCB::wrapper);

	static void wrapper();

private:
	static ID staticID;
	ID id;

	static void idleBody();
};


#endif /* PCB_H_ */
