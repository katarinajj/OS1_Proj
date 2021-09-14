#ifndef LISTTIME_H_
#define LISTTIME_H_

class KernelSem;
class PCB;

typedef unsigned int Time;

class TimeList {
public:
	// ideja Vezbe5.pdf Zad6
	struct Elem {
			Time time;
			PCB *p;
			Elem *next;
			Elem(Time time1, PCB *p1, Elem *next1 = 0) {
				time = time1; p = p1; next = next1;
			}
			~Elem() { time = 0; p = 0; next = 0; }
	};
	Elem *first, *last;
	KernelSem *sem;
	int len;

	TimeList(KernelSem *sem1);
	~TimeList();
	int length();

	void insert(Time time1, PCB *p1);
	void removeTimer();
	PCB* removeAtFront();

	void ispis();

private:
	void deleteList();
};



#endif /* LISTTIME_H_ */
