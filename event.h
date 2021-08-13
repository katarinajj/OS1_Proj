// File: event.h
#ifndef _event_h_
#define _event_h_

#include "ivtEntry.h"

#define PREPAREENTRY(ivtNo, flag)\
void interrupt newISRout##ivtNo(...);\
IVTEntry ivtEntry##ivtNo(ivtNo, newISRout##ivtNo); \
void interrupt newISRout##ivtNo(...) {\
	ivtEntry##ivtNo.signal();\
	if (flag == 1) ivtEntry##ivtNo.oldISR();\
}

/*
#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...); \
IVTEntry newEntry##numEntry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
newEntry##numEntry.signal();\
if (callOld == 1)\
	newEntry##numEntry.oldISR();\
}*/


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
