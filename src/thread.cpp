#include "pcb.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lockCout
	badFork = 0;
	myPCB = new PCB(stackSize, timeSlice, this);
	if (myPCB == 0 || badFork == -1) {
		printf("Nemam memorije u Thread konstruktoru\n");
		badFork = -1;
		myPCB = 0;
	}
	else if (Kernel::allPCBs->insertAtEnd(myPCB) == -1) {
		printf("Nemam memorije u Thread konstr za listu\n");
		if (myPCB) delete myPCB; // ovo ne treba da pravi problem
		badFork = -1;
		myPCB = 0;
	}
	unlockCout
}

Thread::~Thread () {
	lockCout
	if (myPCB) {
		Kernel::allPCBs->removePCB(myPCB);
		delete myPCB;
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

// fork

Thread* childThread = 0;
int badFork = 0;

ID Thread::fork() {
	lockCout

	//printf("----USAO SAM U THREAD::FORK\n");
	badFork = 0;
	childThread = 0;
	if (Kernel::running->myThread == 0) {
		printf("LOSE: Pozvan Thread::Fork a myThread je 0\n");
		unlockCout
		return -1;
	}

	childThread = Kernel::running->myThread->clone();

	if (childThread == 0 || badFork == -1) {
		unlockCout
		return -1;
	}
	if (Kernel::running->myActiveKids->insertAtEnd(childThread->myPCB) == -1) {
		unlockCout
		return -1;
	}


	ID retFork = PCB::fork();
	if (retFork > 0) {
		unlockCout
		return retFork;
	}
	else return 0;
}

void Thread::exit() {
	PCB::exit();
}

void Thread::waitForForkChildren() {
	PCB::waitForForkChildren();
}

Thread* Thread::clone() const {
	lockCout
	printf("Zasto sam u Thread::clone()\n");
	unlockCout
	return 0;
}




