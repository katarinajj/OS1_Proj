
KerSem::KerSem(int init, Semaphore *mySem) {
	this->value = init;
	this->mySem = mySem;
	
	lockCout
	blockedPCBs = new List();
	waitingPCBs = new TimeList(this);	
	unlockCout
}

KerSem::~KerSem() {

}

int KerSem::wait(Time maxTimeToWait) {
	if (maxTimeToWait == 0) {
		lockCout
		if (--val < 0) block();
		unlockCout
		return 1;
		
	}
	else {
		
	}
}

void KerSem::signal() {
	lockCout
	if (++val <= 0) unblock();
	unlockCout
}

int KerSem::val() const {
	return this->value;
}

void KerSem::block() {
	lockCout
	running->state = SUSPENDED;
	blockedPCBs.insertAtEnd((PCB*) running);
	context_switch_on_demand = 1;
	unlockCout
	dispatch();
}

void KerSem::unblock() {
	PCB *tmp = blockedPCBs.removeAtFront();
	tmp->state = READY;
	Scheduler::put(tmp);
}