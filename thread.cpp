#include "pcb.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lockCout
	myPCB = new PCB(stackSize, timeSlice, this);
	if (myPCB == 0) printf("Nemam memorijeeee\n"); //TODO: ovo
	else Kernel::allPCBs->insertAtEnd(myPCB);
	//Kernel::allPCBs->ispis();
	unlockCout
}

Thread::~Thread () {
	lockCout
	if (myPCB) {
		Kernel::allPCBs->removePCB(myPCB);
		delete myPCB; //TODO: pazi
		myPCB = 0;
	}
	unlockCout
}

void Thread::start() {
	if (myPCB) myPCB->start();
}

void Thread::waitToComplete() {
	if (myPCB) myPCB->waitToComplete();
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





