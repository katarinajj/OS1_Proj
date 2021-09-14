#include "kerEv.h"


KernelEv::KernelEv(IVTNo ivtNo, PCB *creator) {
	lockCout
	this->value = 0;
	this->myIVTEntry = ivtNo;
	this->creatorPCB = creator;
	Kernel::ivtEntries[ivtNo]->myKerEv = this;
	unlockCout
}

KernelEv::~KernelEv() {
	lockCout
	this->creatorPCB = 0;
	if (Kernel::ivtEntries[this->myIVTEntry]) Kernel::ivtEntries[this->myIVTEntry]->resetKernelEv();
	unlockCout
}

// Programiranje u realnom vremenu skripta

void KernelEv::wait() {
	lockCout
	if (Kernel::running == this->creatorPCB) {

		if (--value < 0) {
			Kernel::running->state = SUSPENDED;
			unlockCout
			dispatch();
		}
		else {
			unlockCout
		}

	}
	else {
		unlockCout
	}
}

void KernelEv::signal() {
	lockCout

	if (++value <= 0) {
		creatorPCB->state = READY;
		Scheduler::put((PCB*) creatorPCB);
	}
	else value = 1;

	unlockCout
}




