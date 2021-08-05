#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class Idle : public Thread {
public:
	Idle (StackSize stackSize, Time timeSlice) : Thread(256, 1) {}
	void run() { while(1); }
	~Idle() { waitToComplete(); }
};


#endif /* IDLE_H_ */
