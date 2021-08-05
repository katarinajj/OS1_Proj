#include <iostream.h>

#include "timer.h"

PCB *p[3];

void exitThread(){
	PCB::running->state = TERMINATED;
	dispatch();
}

void a(){
	for (int i =0; i < 30; ++i) {
		lockCout
		cout<<"u a() i = "<<i<<endl;
		unlockCout
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		lockCout
		cout<<"u b() i = "<<i<<endl;
		unlockCout
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void doSomething(){
	lock
	p[1] = new PCB(4096, 40, 0, a);
	cout<<"napravio a"<<endl;
	Scheduler::put(p[1]);

	p[2] = new PCB(4096, 20, 0, b);
	cout<<"napravio b"<<endl;
	Scheduler::put(p[2]);

	p[0] = new PCB(4096, 20, 0);

	PCB::running = p[0];
	unlock

#ifndef BCC_BLOCK_IGNORE
	for (int i = 0; i < 30; ++i) {
		lock
		cout<<"main "<<i<<endl;
		unlock

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
#endif
	cout<<"Happy End"<<endl;
}

int main(){

  inic();

  doSomething();

  restore();

  if (PCB::allPCBs) delete PCB::allPCBs;
  return 0;
}
