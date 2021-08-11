#include <dos.h>
#include "pcb.h"

void idleBody() {
	while(1) {
		//printf("IDLE\n");
	}
}

ID PCB::staticID = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)()) {
	// TODO: ispravi ovo za velicinu steka
	if (stackSize > maxStackSize) stackSize = maxStackSize;

	unsigned long numOfIndex = stackSize / sizeof(unsigned);

	lockCout
	unsigned* st1 = new unsigned[numOfIndex];
	unlockCout

	st1[numOfIndex - 1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit

#ifndef BCC_BLOCK_IGNORE
	st1[numOfIndex - 2] = FP_SEG(body);
	st1[numOfIndex - 3] = FP_OFF(body);

	this->ss = FP_SEG(st1 + numOfIndex - 12);
	this->sp = FP_OFF(st1 + numOfIndex - 12);  //svi sacuvani reg pri ulasku u interrupt rutinu
    this->bp = FP_OFF(st1 + numOfIndex - 12);
#endif

	this->state = INITIALIZED;
	this->myThread = myThread;
	this->timeSlice = timeSlice;
	this->stack = st1;
	this->unblockedByTime = 0;

	lockCout
	this->id = ++staticID;
	this->waitingForThis = new List();
	unlockCout
}

PCB::PCB() {
	this->ss = this->sp = this->bp = 0;
	this->stack = 0;
	this->myThread = 0;
	this->waitingForThis = 0;
	this->unblockedByTime = 0;

	this->timeSlice = defaultTimeSlice;
	this->state = READY; // TODO: proveri je l bitno

	lockCout
	this->id = ++staticID;
	unlockCout
}

void PCB::start() {
	lockCout
	if (this->state == INITIALIZED) {
		this->state = READY;
		Scheduler::put(this);
	}
	unlockCout
}

void PCB::waitToComplete() {
	lockCout
	if (this->state != TERMINATED) {
		Kernel::running->state = SUSPENDED;
		waitingForThis->insertAtEnd((PCB*)Kernel::running);
		unlockCout
		dispatch();
	}
	else {
		unlockCout
	}
}

PCB::~PCB() {
	lockCout
	if(this->stack) {
		delete [] this->stack;
		this->stack = 0;
	}

	unlockCout
}

ID PCB::getId() { return id; }

ID PCB::getRunningId() {
	return Kernel::running->id;
}

Thread * PCB::getThreadById(ID id) {
	PCB *tmp = 0;
	int found = 0;
	lockCout
	for(Kernel::allPCBs->onFirst(); Kernel::allPCBs->hasCur(); Kernel::allPCBs->onNext()) {
		tmp = (PCB*)(Kernel::allPCBs->getCur());
		if (tmp->getId() == id) { found = 1; break; }
	}
	unlockCout
	if (found == 0) return 0;
	else return tmp->myThread;
}

void PCB::wrapper() {
	Kernel::running->myThread->run();
	lockCout
	PCB *tmp = (PCB*)(Kernel::running->waitingForThis->removeAtFront());
	while (tmp != 0) {
		tmp->state = READY;
		Scheduler::put(tmp);
		tmp = (PCB*)(Kernel::running->waitingForThis->removeAtFront());
	}
	Kernel::running->state = TERMINATED;
	unlockCout
	dispatch();
}



