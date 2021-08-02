#include "thread.h"
#include "pcb.h"
#include "schedule.h"


Thread::Thread (StackSize stackSize, Time timeSlice) {
	this->myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread () {
	if (myPCB) delete myPCB;
	myPCB = 0;
}

void Thread::waitToComplete() {
	if (myPCB) myPCB->waitToComplete();
}


void dispatch() {
	asm cli;
	context_switch_on_demand = 1;
	timer();
	asm sti;
}

void Thread::start() {
	if (myPCB) myPCB->start();
}

ID Thread::getId() {
	if (myPCB) myPCB->getId();
	else return -1;
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread * Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

