#include "kerEv.h"


KernelEv::KernelEv(IVTNo ivtNo, PCB *creator) {
	lockCout
	this->value = 0;
	this->hasBlocked = 0;
	this->creator = creator;
	Kernel::ivtEntries[ivtNo]->setKernelEv(this);
	// ivtNo-om ulazu prosledim pokazivac na this da li za to treba lock
	unlockCout
}

KernelEv::~KernelEv() { }

void KernelEv::wait() {
	if (Kernel::running == this->creator) {
		lockCout
		if (this->value == 0) {
			Kernel::running->state = SUSPENDED;
			this->hasBlocked = 1;
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
	if (hasBlocked == 0) this->value = 1;
	else {
		Kernel::running->state = READY;
		Scheduler::put((PCB*) Kernel::running);
		hasBlocked = 0;
	}
	unlockCout
}




