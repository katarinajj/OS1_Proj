#include "pcb.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	this->myPCB = new PCB(stackSize, timeSlice, this); // run ovako??
	PCB::allPCBs->addPCB(this->myPCB);
}

Thread::~Thread () {
	if (myPCB) PCB::allPCBs->removePCB(myPCB);
	myPCB = 0;
}

void Thread::waitToComplete() {
	if (myPCB) myPCB->waitToComplete();
}

void Thread::start() {
	if (myPCB) myPCB->start();
}

ID Thread::getId() {
	if (myPCB) return myPCB->getId();
	else return -1;
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread * Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}





