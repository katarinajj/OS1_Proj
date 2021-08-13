#include "kerEv.h"


Event::Event(IVTNo ivtNo) {
	lockCout
	myImpl = new KernelEv(ivtNo, (PCB*) Kernel::running);
	unlockCout
}

Event::~Event() {
	lockCout
	if (myImpl) { delete myImpl; myImpl = 0; }
	unlockCout
}

void Event::wait() {
	if (myImpl) myImpl->wait();
}

void Event::signal() {
	if (myImpl) myImpl->signal();
}
