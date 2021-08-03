#include <iostream>
#include <dos.h>

#include "pcb.h"
#include "SCHEDULE.h"

int PCB::staticID = 0;

idleThread = new PCB(defaultStackSize, 1, 0); //idleBody
 

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	if (stackSize < defaultStackSize) stackSize = defaultStackSize;
	else if (stackSize > maxStackSize) stackSize = maxStackSize
	unsigned long numOfIndex = stackSize / sizeof(unsigned);
	unsigned* st1 = new unsigned[numOfIndex];
	
	st1[numOfIndex - 1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit
	st1[numOfIndex - 2] = FP_SEG(PCB::wrapper);
	st1[numOfIndex - 3] = FP_OFF(PCB::wrapper);
	
	this->ss = FP_SEG(st1 + numOfIndex - 12);
	this->sp = FP_OFF(st1 + numOfIndex - 12);  //svi sacuvani reg pri ulasku u interrupt rutinu
    this->bp = FP_OFF(st1 + numOfIndex - 12); 
   
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
		waitingForThis.add(running);		
		dispatch();
	}
	
}

PCB::~PCB() {
	delete this->stack;
	delete waitingForThis;
	delete myThread;
}

ID PCB::getId() { return id; }

ID PCB::getRunningId() {
	return running->getId();
}

Thread * PCB::getThreadById(ID id) {
	Elem *tmp = first;
	for (Elem *tmp = everyPCB.first; tmp && tmp->p->getId() != id; tmp = tmp->next);

	if (!tmp) return 0; // nije ni bio u listi
	return tmp->myThread;
}

void idleBody() {
	while(1);
}

void PCB::wrapper() {
	
	PCB::running->myThread->run();
	for (Elem *tmp = waitingForThis.first; tmp; tmp = tmp->next) {
		tmp->p->state = READY;
		Scheduler::put(tmp->p);
	}
	waitingForThis.deleteList();
	PCB::running->state = TERMINATED;
	dispatch();
}