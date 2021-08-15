#include "kerEv.h"


KernelEv::KernelEv(IVTNo ivtNo, PCB *creator) {
	lockCout
	this->value = 0;
	this->myIVTEntry = ivtNo;
	this->blockedPCB = 0;
	this->creatorPCB = creator;
	Kernel::ivtEntries[ivtNo]->setKernelEv(this);
	// ivtNo-om ulazu prosledim pokazivac na this da li za to treba lock
	unlockCout
}

KernelEv::~KernelEv() {
	Kernel::ivtEntries[this->myIVTEntry]->resetKernelEv();
}

void KernelEv::wait() {
	if (Kernel::running == this->creatorPCB) {
		lockCout
		if (this->value == 0) {
			Kernel::running->state = SUSPENDED;
			this->blockedPCB = (PCB*) Kernel::running;
			unlockCout
			dispatch();
		}
		else {
			this->value = 0;
			unlockCout
		}
	}
}

void KernelEv::signal() {
	lockCout
	if (blockedPCB == 0) this->value = 1;
	else {
		blockedPCB->state = READY;
		Scheduler::put((PCB*) blockedPCB);
		blockedPCB = 0;
	}
	unlockCout
}




