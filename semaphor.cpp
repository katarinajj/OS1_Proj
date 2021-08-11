#include "kerSem.h"

Semaphore::Semaphore(int init) {
	lockCout
	if (init < 0) init = 0;
	myImpl = new KernelSem(init, this);
	Kernel::allKernelSems->insertAtEnd(myImpl);
	Kernel::allKernelSems->ispis();
	unlockCout
}

Semaphore::~Semaphore() {
	lockCout
	if (myImpl) {
		Kernel::allKernelSems->removeKernelSem(myImpl);
		delete myImpl;
		myImpl = 0;
	}
	unlockCout
}

int Semaphore::wait(Time maxTimeToWait) {
	if (myImpl) return myImpl->wait(maxTimeToWait);
	else return -1; // TODO: ?
}

void Semaphore::signal() {
	if (myImpl) myImpl->signal();
}

int Semaphore::val() const {
	if (myImpl) return myImpl->val();
	else return -1; // TODO: ?
}



