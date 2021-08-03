#include <dos.h>

#include "pcb.h"

ID PCB::staticID = 0;

volatile PCB *running = 0;

void idleBody() {
	while(1);
}

PCB* PCB::idleThread = new PCB(defaultStackSize, 1, 0, idleBody);


PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)()) {
	if (stackSize < defaultStackSize) stackSize = defaultStackSize;
	else if (stackSize > maxStackSize) stackSize = maxStackSize;
	unsigned long numOfIndex = stackSize / sizeof(unsigned);
	unsigned* st1 = new unsigned[numOfIndex];

	st1[numOfIndex - 1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit

#ifndef BCC_BLOCK_IGNORE
	st1[numOfIndex - 2] = FP_SEG(body);
	st1[numOfIndex - 3] = FP_OFF(body);

	this->ss = FP_SEG(st1 + numOfIndex - 12);
	this->sp = FP_OFF(st1 + numOfIndex - 12);  //svi sacuvani reg pri ulasku u interrupt rutinu
    this->bp = FP_OFF(st1 + numOfIndex - 12);
#endif

	this->state = INITIALIZED;
	this->id = ++staticID;

}

void PCB::start() {
	this->state = READY;
	Scheduler::put(this);
}

void PCB::waitToComplete() {
	if (this->state != TERMINATED) {
		running->state = SUSPENDED;
		//waitingForThis.add(running);
		dispatch();
	}

}

PCB::~PCB() {
	delete this->stack;
	//delete waitingForThis;
	delete myThread;
}

ID PCB::getId() { return id; }

ID PCB::getRunningId() {
	//return running->getId(); ???
	return 0;
}

Thread * PCB::getThreadById(ID id) {
	/*
	Elem *tmp = first;
	for (Elem *tmp = everyPCB.first; tmp && tmp->p->getId() != id; tmp = tmp->next);

	if (!tmp) return 0; // nije ni bio u listi
	return tmp->myThread;
	*/
	return 0;
}

void PCB::wrapper() {

	running->myThread->run();
	/*
	for (Elem *tmp = waitingForThis.first; tmp; tmp = tmp->next) {
		tmp->p->state = READY;
		Scheduler::put(tmp->p);
	}
	waitingForThis.deleteList();
	*/
	running->state = TERMINATED;
	dispatch();
}



