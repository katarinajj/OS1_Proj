#ifndef KEREV_H_
#define KEREV_H_

#include "pcb.h"
#include "ivtEntry.h"
#include "event.h"

class KernelEv {
public:

	PCB *creator;

	KernelEv (IVTNo ivtNo, PCB *creator);
	~KernelEv ();

	void wait();
	void signal();

private:
	int value;
	int hasBlocked;

};




#endif /* KEREV_H_ */
