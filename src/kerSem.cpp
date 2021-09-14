#include "kerSem.h"

KernelSem::KernelSem(int init, Semaphore *mySem) {
	this->value = init;
	this->mySem = mySem;

	lockCout
	this->blockedPCBs = new List();
	this->waitingPCBs = new TimeList(this);
	unlockCout
}

KernelSem::~KernelSem() {
	lockCout
	delete blockedPCBs;
	delete waitingPCBs;
	unlockCout
}

int KernelSem::wait(Time maxTimeToWait) {
	lockCout
	int ret = 0;
	if (--value < 0) {
		if (maxTimeToWait == 0) { unlockCout; ret = block(); }
		else { unlockCout; ret = blockTime(maxTimeToWait); }
	}
	else { unlockCout; ret = 1; }
	return ret;
}

void KernelSem::signal() {
	lockCout
	if (++value <= 0) unblock();
	unlockCout
}

int KernelSem::val() const { return this->value; }


int KernelSem::block() {
	lockCout
	Kernel::running->state = SUSPENDED;
	blockedPCBs->insertAtEnd((PCB*) Kernel::running);
	//blockedPCBs->ispis();
	unlockCout
	dispatch();
	return 1;
}

int KernelSem::blockTime(Time maxTimeToWait) {
	lockCout
	Kernel::running->state = SUSPENDED;
	waitingPCBs->insert(maxTimeToWait, (PCB*) Kernel::running);
	//waitingPCBs->ispis();
	unlockCout
	dispatch();

	lockCout
	if (Kernel::running->unblockedByTime == 1) {
		Kernel::running->unblockedByTime = 0;
		unlockCout
		return 0;
	}
	else { unlockCout; return 1; }
}

void KernelSem::unblock() {
	PCB *tmp = (PCB*)(blockedPCBs->removeAtFront());
	if (!tmp) tmp = waitingPCBs->removeAtFront();

	tmp->state = READY;
	Scheduler::put(tmp);
}



