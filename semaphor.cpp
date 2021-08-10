
Semaphore::Semaphore(int init) {
	lockCout
	myImpl = new KerSem(init, this);
	allKerSems->insertAtEnd(myImpl);
	unlockCout
}

Semaphore::~Semaphore() {
	lockCout
	if (myImpl) {
		allKerSems->removeKerSem(myImpl);
		delete myImpl;
		myImpl = 0;
	}
	unlockCout
}

int Semaphore::wait(Time maxTimeToWait) {
	if (myImpl) return myImpl->wait(maxTimeToWait);
}

void Semaphore::signal() {
	if (myImpl) myImpl->signal();
}

int Semaphore::val() const {
	if (myImpl) return myImpl->val();
}