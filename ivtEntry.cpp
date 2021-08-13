#include "kerEv.h"
#include "asystem.h"


IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newISR) {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	this->ivtNo = ivtNo;
	this->oldISR = getvect(this->ivtNo);
	setvect(this->ivtNo, newISR);
	unlock
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	setvect(this->ivtNo, this->oldISR);
	unlock
}

void IVTEntry::signal() {
	if (myKerEv) myKerEv->signal();
}

void IVTEntry::setKernelEv(KernelEv *ke) {
	this->myKerEv = ke;
}



