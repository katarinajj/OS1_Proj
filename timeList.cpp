#include "kerSem.h"
#include "pcb.h"

void TimeList::deleteList() {
	Elem *old = first;
	while (first) {
		old = first;
		first = first->next;
		lockCout
		delete old;
		unlockCout
	}
	first = 0;
	sem = 0;
	len = 0;
}

TimeList::TimeList(KerSem *sem1) {
	first = 0;
	sem = sem1;
	len = 0;
}

TimeList::~TimeList() { deleteList(); }

int TimeList::length() { return len; }

void TimeList::insert(Time time1, PCB *p1) {
	lockCout
	Elem *tmp = new Elem(time1, p1);
	len++;
	if (!first) first = tmp;
	else if (time1 < first->time) { // treba da ga umetnem kao novu glavu
		first->time = first->time - time1;
		tmp->next = first;
		first = tmp;
	}
	else { // time1 > first->time		
		Time curTime = first->time, prevTime = curTime;
		Elem *cur = first->next, *prev = first;
		while (cur) {
			curTime += cur->time;
			if (time1 >= curTime) {
				prevTime = curTime; prev = cur; cur = cur->next;
			}
			else break;
		}
		tmp->time = time1 - prevTime;
		prev->next = tmp;
		if (cur) tmp->next = cur;
	}
	unlockCout
}

void TimeList::remove() {
	if (!first) return;
	lockCout
	first->time--;
	while (first && first->time == 0) {
		first->p->state = READY;
		Scheduler::put(first->p);
		//sem->signal();
		first = first->next;
	}
	Elem* tmp = first;
	while (tmp) {
		tmp->time--;
		tmp = tmp->next;
	}
	unlockCout
	
}




