#ifndef IVTENTRY_H_
#define IVTENTRY_H_


typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

class KernelEv;


class IVTEntry {
public:

	IVTEntry(IVTNo ivtNo, pInterrupt newISR);
	~IVTEntry();

	void signal();
	void resetKernelEv();

	pInterrupt oldISR;
	KernelEv *myKerEv;

private:

	IVTNo ivtNo;
};



#endif /* IVTENTRY_H_ */
