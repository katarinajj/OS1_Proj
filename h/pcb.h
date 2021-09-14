#ifndef PCB_H_
#define PCB_H_

#include "asystem.h"
#include "list.h"
#include "thread.h"
#include "SCHEDULE.h"

const StackSize maxStackSize = 65535;

enum State {INITIALIZED, READY, SUSPENDED, TERMINATED, WAIT4KIDS};

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

	// fork
	static ID fork();
	static void exit();
	static void waitForForkChildren();
	static void interrupt copyStack();

	List *myActiveKids;
	PCB *parent;
	StackSize stackSize;

private:
	static ID staticID;
	static void breakBondsWithKids();

};

// fork globalne promenljive

extern Thread* childThread;
extern int badFork;

extern int diffOff;
//extern int diffSeg;
extern unsigned long numOfIndex2;

extern unsigned curSS;
extern unsigned curSP;
extern unsigned curBPOff;
extern unsigned *curBPAdr;
extern unsigned *childBPAdr;


#endif /* PCB_H_ */
