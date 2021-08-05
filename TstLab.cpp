#include <iostream.h>
#include "timer.h"

class TestA : public Thread {
public:
	TestA (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize, timeSlice) {}
	void run();
	~TestA() { waitToComplete(); }
};

class TestB : public Thread {
public:
	TestB (StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice) : Thread(stackSize, timeSlice) {}
	void run();
	~TestB() {waitToComplete();}
};

void TestA::run(){
	for (int i =0; i < 30; ++i) {
		lockCout
		cout<<"u a() i = "<<i<<endl;
		unlockCout
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

void TestB::run(){
	for (int i =0; i < 30; ++i) {
		lockCout
		cout<<"u b() i = "<<i<<endl;
		unlockCout
		for (int k = 0; k<30000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

int userMain(int argc, char* argv[])
{
	lockCout

	TestA* p1 = new TestA(defaultStackSize, 40);

	lock
	cout<<"Napravio a"<<endl;
	unlock

	TestB* p2 = new TestB(defaultStackSize, 20);

	lock
	cout<<"Napravio b"<<endl;
	unlock

	unlockCout

	p1->start();
	p2->start();
	for (int i = 0; i < 30; ++i) {
		lock
		cout<<"main "<<i<<endl;
		unlock

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
	p1->waitToComplete();
	p2->waitToComplete();

	lockCout
	delete p1;
	delete p2;
	unlockCout

	lock
	cout<<"Srecan kraj!"<<endl;
	unlock

	return 0;
}
