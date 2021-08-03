#include "pcb.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	this->myPCB = new PCB(stackSize, timeSlice, this); // run ovako??
	//everyPBC.add(this->myPCB);
}

Thread::~Thread () {
	//if (myPCB) everyPCB.remove(myPCB);
	myPCB = 0;
}

void Thread::waitToComplete() {
	if (myPCB) myPCB->waitToComplete();
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





