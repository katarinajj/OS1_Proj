#include "kerEv.h"
#include "asystem.h"


IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newISR) {
	this->ivtNo = ivtNo;
	Kernel::ivtEntries[this->ivtNo] = this;
#ifndef BCC_BLOCK_IGNORE
	lock
	this->oldISR = getvect(this->ivtNo);
	setvect(this->ivtNo, newISR);
	unlock
#endif
}

IVTEntry::~IVTEntry() {
	Kernel::ivtEntries[this->ivtNo] = 0;
	resetKernelEv();
}

void IVTEntry::signal() {
	if (this->myKerEv) {
		myKerEv->signal();
	}
}

void IVTEntry::resetKernelEv() {
	this->myKerEv = 0;

#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(this->ivtNo, this->oldISR);
	unlock
#endif
}



