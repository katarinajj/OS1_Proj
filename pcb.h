#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "list.h"

const StackSize stackSize = 65536;

enum State {CREATED, READY, RUNNING, BLOCKED, FINISHED, IDLE};

class PCB {
public:

	Thread *myThread; 
	List<PCB*> waitingForThis;
	
	~PCB();
	void start();
	void waitToComplete();
	virtual ~Thread();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

protected:
	PCB (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread *myThread); // void (*fun)() = PCB::runner
	virtual void run() {}

private:
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	State state;
	unsigned *stack;
};

void dispatch();

#endif