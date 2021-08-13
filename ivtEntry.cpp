#include "kerEv.h"
#include "asystem.h"


IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt newISR) {
#ifndef BCC_BLOCK_IGNORE
	lock
	this->ivtNo = ivtNo;
	this->oldISR = getvect(this->ivtNo);
	setvect(this->ivtNo, newISR);
	Kernel::ivtEntries[ivtNo] = this;
	unlock
#endif
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	lock
	setvect(this->ivtNo, this->oldISR);
	unlock
#endif
}

void IVTEntry::signal() {
	if (this->myKerEv) {
		myKerEv->signal();
	}
}

void IVTEntry::setKernelEv(KernelEv *ke) {
	this->myKerEv = ke;
	//cout << "\n\nSETOVAN ker ev  ---" << myKerEv << "NULL: " << (myKerEv == 0) << endl;
}



