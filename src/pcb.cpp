#include "pcb.h"

void idleBody() {
	while(idleLoopConst) {
		//printf("IDLE\n");
	}
}

ID PCB::staticID = 0;


PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)()) {
	if (stackSize > maxStackSize) stackSize = maxStackSize;

	unsigned long numOfIndex = stackSize / sizeof(unsigned);

	lockCout
	unsigned* st1 = new unsigned[numOfIndex];
	unlockCout

	if (!st1) {
		printf("Nemam memorije za stek u PCB konstr\n");
		badFork = -1;
	}
	else {
		st1[numOfIndex - 1] = 0x200;

	#ifndef BCC_BLOCK_IGNORE
		st1[numOfIndex - 2] = FP_SEG(body);
		st1[numOfIndex - 3] = FP_OFF(body);

		this->ss = FP_SEG(st1 + numOfIndex - 12);
		this->sp = FP_OFF(st1 + numOfIndex - 12);
		this->bp = FP_OFF(st1 + numOfIndex - 12);
	#endif

		this->state = INITIALIZED;
		this->myThread = myThread;
		this->timeSlice = timeSlice;
		this->stack = st1;
		this->unblockedByTime = 0;

		// fork
		st1[numOfIndex - 12] = 0;
		this->stackSize = stackSize;
		this->parent = 0;

		lockCout
		this->id = ++staticID;
		this->waitingForThis = new List();
		this->myActiveKids = new List();
		if (this->waitingForThis == 0 || this->myActiveKids == 0) {
			printf("Nemam memorije za liste u PCB konstr\n");
			badFork = -1;
		}
		unlockCout
	}
}

PCB::PCB() {
	this->ss = this->sp = this->bp = 0;
	this->stack = 0;
	this->myThread = 0;
	this->waitingForThis = 0;
	this->unblockedByTime = 0;

	this->timeSlice = 0;
	this->state = READY;

	// fork
	this->stackSize = 0;
	this->parent = 0;
	this->myActiveKids = 0;

	lockCout
	this->id = ++staticID;
	unlockCout
}

void PCB::start() {
	lockCout
	if (this->state == INITIALIZED) {
		++numOfUnfinishedPCBs;
		this->state = READY;
		Scheduler::put(this);
	}
	unlockCout
}

void PCB::waitToComplete() {
	lockCout
	if (this->state != TERMINATED && this->state != INITIALIZED && Kernel::running != this && this != Kernel::idlePCB) {
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

	if (this->stack) {
		delete [] this->stack;
		this->stack = 0;
	}
	if (this->myActiveKids) {
		delete this->myActiveKids;
		this->myActiveKids = 0;
	}
	if (this->waitingForThis) {
		delete this->waitingForThis;
		this->waitingForThis = 0;
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
	if (found == 0) {
		unlockCout
		return 0;
	}
	else {
		unlockCout
		return tmp->myThread;
	}
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

	// fork
	PCB::breakBondsWithKids();

	Kernel::running->state = TERMINATED;
	--numOfUnfinishedPCBs;
	unlockCout
	dispatch();
}


// fork

int diffOff = 0;
unsigned long numOfIndex2 = 0;

ID PCB::fork() {
	lockCout

	childThread->myPCB->parent = (PCB*)Kernel::running;
	PCB* runningParent = (PCB*)Kernel::running;
	ID retID = childThread->myPCB->id;

#ifndef BCC_BLOCK_IGNORE
	diffOff = FP_OFF(childThread->myPCB->stack) - FP_OFF(Kernel::running->stack);
#endif

	numOfIndex2 = Kernel::running->stackSize / sizeof(unsigned);

	PCB::copyStack();

	if (Kernel::running == runningParent) {
		unlockCout
		return retID;
	}
	else return 0;

}

unsigned curSS;
unsigned curSP;
unsigned curBPOff;
unsigned *curBPAdr;
unsigned *childBPAdr;

void interrupt PCB::copyStack() {
	for (unsigned i = 0; i < numOfIndex2; ++i) {
		childThread->myPCB->stack[i] = Kernel::running->stack[i];
	}

#ifndef BCC_BLOCK_IGNORE
	asm {
		mov curSS, ss
		mov curSP, sp
		mov curBPOff, bp
	}
#endif

	childThread->myPCB->bp = curBPOff + diffOff;

	//azuriranje BP deteta
	while (curBPOff != 0) {
#ifndef BCC_BLOCK_IGNORE
		childBPAdr = (unsigned*)MK_FP(childThread->myPCB->ss, curBPOff + diffOff);
		*childBPAdr += diffOff;
		curBPAdr = (unsigned*)MK_FP(curSS, curBPOff);
#endif
		curBPOff = *curBPAdr;
	}

	childThread->myPCB->sp = childThread->myPCB->bp;

	// startujem nit dete
	if (childThread->myPCB->state == INITIALIZED) {
		++numOfUnfinishedPCBs;
		childThread->myPCB->state = READY;
		Scheduler::put(childThread->myPCB);

	}
}

void PCB::exit() {
	lockCout
	// deo kao za wrapper
	PCB *tmp = (PCB*)(Kernel::running->waitingForThis->removeAtFront());
	while (tmp != 0) {
		tmp->state = READY;
		Scheduler::put(tmp);
		tmp = (PCB*)(Kernel::running->waitingForThis->removeAtFront());
	}

	// fork
	PCB::breakBondsWithKids();

	Kernel::running->state = TERMINATED;
	--numOfUnfinishedPCBs;
	unlockCout
	dispatch();
}

void PCB::breakBondsWithKids() {
	// ako si dete obavesti roditelja da si gotov

	if (Kernel::running->parent) {

		Kernel::running->parent->myActiveKids->removePCB((PCB*)Kernel::running);

		if (Kernel::running->parent->state == WAIT4KIDS && Kernel::running->parent->myActiveKids->len == 0) {
			Kernel::running->parent->state = READY;
			Scheduler::put(Kernel::running->parent);
		}
	}

	// ako si roditelj raskines vezu sa aktivnom decom
	for(Kernel::running->myActiveKids->onFirst(); Kernel::running->myActiveKids->hasCur(); Kernel::running->myActiveKids->onNext()) {
		((PCB*)(Kernel::running->myActiveKids->getCur()))->parent = 0;
	}

}

void PCB::waitForForkChildren() {
	lockCout
	if (Kernel::running->myActiveKids && Kernel::running->myActiveKids->len > 0) {

		Kernel::running->state = WAIT4KIDS;

		unlockCout
		dispatch();
	}
	else {
		unlockCout
	}
}


