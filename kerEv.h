#ifndef KEREV_H_
#define KEREV_H_

#include "pcb.h"
#include "ivtEntry.h"
#include "event.h"

class KernelEv {
public:

	KernelEv (IVTNo ivtNo, PCB *creator);
	~KernelEv ();

	void wait();
	void signal();

private:
	int value;
	PCB* blockedPCB;
	PCB* creatorPCB;
	IVTNo myIVTEntry;

};




#endif /* KEREV_H_ */
