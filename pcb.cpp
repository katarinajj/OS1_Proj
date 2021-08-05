#include <dos.h>
#include "pcb.h"

ID PCB::staticID = 0;

volatile PCB* PCB::running = 0;
List* PCB::allPCBs = new List();

void PCB::idleBody() {
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
	this->myThread = myThread;
	this->timeSlice = timeSlice;
	this->stack = st1;
	this->waitingForThis = new List();
}

void PCB::start() {
	this->state = READY;
	Scheduler::put(this);
}

void PCB::waitToComplete() {
	if (this->state != TERMINATED) {
		running->state = SUSPENDED;
		waitingForThis->addPCB((PCB*)running);
		dispatch();
	}
}

PCB::~PCB() {
	if(this->stack) delete this->stack;
	if(myThread) delete myThread;
}

ID PCB::getId() { return id; }

ID PCB::getRunningId() {
	return running->id;
}
/*
 * PCB* List::getPCBbyId(ID id) {
	Elem *tmp = first;
	for (; tmp && ((PCB*)(tmp->p))->getId() != id; tmp = tmp->next);

	if (!tmp) return 0; // nije ni bio u listi
	else return (PCB*)tmp->p;
}
 */


Thread * PCB::getThreadById(ID id) {
	PCB *tmp = 0;
	int found = 0;
	for(allPCBs->onFirst(); allPCBs->hasCur(); allPCBs->onNext()) {
		tmp = (PCB*)(allPCBs->getCur());
		if (tmp->getId() == id) { found = 1; break; }
	}
	if (found == 0) return 0;
	else return tmp->myThread;
}

void PCB::wrapper() {
	running->myThread->run();
	PCB *tmp = 0;
	for (running->waitingForThis->onFirst(); running->waitingForThis->hasCur(); running->waitingForThis->onNext()) {
		tmp = (PCB*)(running->waitingForThis->getCur());
		tmp->state = READY;
		Scheduler::put(tmp);
	}
	running->waitingForThis->deleteList();
	running->state = TERMINATED;
	dispatch();
}



