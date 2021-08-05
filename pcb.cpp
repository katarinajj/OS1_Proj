#include <dos.h>
#include "pcb.h"

void idleBody() {
	while(1);
}

ID PCB::staticID = 0;

PCB* PCB::mainPCB = new PCB();

PCB* PCB::idlePCB = new PCB(defaultStackSize, 1, 0, idleBody);

volatile PCB* PCB::running = mainPCB;

List* PCB::allPCBs = new List();

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)()) {
	// TODO: ispravi ovo za velicinu steka
	if (stackSize < defaultStackSize) stackSize = defaultStackSize;
	else if (stackSize > maxStackSize) stackSize = maxStackSize;

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
	this->id = ++staticID;
	this->myThread = myThread;
	this->timeSlice = timeSlice;
	this->stack = st1;

	lockCout
	this->waitingForThis = new List();
	unlockCout
}

PCB::PCB() { // konstruktor za mainPCB
	this->ss = this->sp = this->bp = 0;
	this->stack = 0;

	this->timeSlice = defaultTimeSlice;
	this->state = READY; // ?

	this->myThread = 0;
	this->waitingForThis = 0;
	this->id = ++staticID;
}

void PCB::start() {
	if (this->state == INITIALIZED) {
		this->state = READY;
		Scheduler::put(this);
	}
}

void PCB::waitToComplete() {
	if (this->state != TERMINATED) {
		running->state = SUSPENDED;
		waitingForThis->addPCB((PCB*)running);
		dispatch();
	}
}

PCB::~PCB() {
	if(this->stack) {
		lockCout
		delete [] this->stack;
		unlockCout
		this->stack = 0;
	}
	//if(myThread) delete myThread;
}

ID PCB::getId() { return id; }

ID PCB::getRunningId() {
	return running->id;
}

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



