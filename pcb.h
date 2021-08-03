#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include "list.h"

const StackSize maxStackSize = 65536;

enum State {INITIALIZED, READY, RUNNING, SUSPENDED, TERMINATED};

PCB *idleThread;
volatile PCB *running;
volatile List everyPCB;

class PCB {
public:
	unsigned ss;
	unsigned sp;
	unsigned bp;
	unsigned timeSlice;
	State state;
	unsigned *stack;
	Thread *myThread; 
	List waitingForThis;
	
	void start();
	void waitToComplete();
	~PCB();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	PCB (StackSize stackSize, Time timeSlice, Thread *myThread); 
	
	static void wrapper();
private:
	static staticID;
	ID id;
};


#endif