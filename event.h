// File: event.h
#ifndef _event_h_
#define _event_h_

#include "ivtEntry.h"

#define PREPAREENTRY(ivtNo, flag)\
void interrupt newISRout##ivtNo(...);\
IVTEntry ivtEntry##ivtNo(ivtNo, newISRout##ivtNo);\
void interrupt newISRout##ivtNo(...) {\
	ivtEntry##ivtNo.signal();\
	if (flag == 1) ivtEntry##ivtNo.oldISR();\
}


typedef unsigned char IVTNo;

class KernelEv;

class Event {

public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv

private:
	KernelEv* myImpl;
};
#endif
