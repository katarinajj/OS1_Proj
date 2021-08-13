#include "pcb.h"
#include "kerSem.h"
#include "iostream.h"

TimeList::TimeList(KernelSem *sem1) {
	this->sem = sem1;
	first = last = 0;
	len = 0;
}

void TimeList::deleteList() {
	Elem *old = first;
	while (first) {
		old = first;
		first = first->next;
		lockCout
		delete old;
		unlockCout
	}
	first = last = 0;
	sem = 0;
	len = 0;
}

TimeList::~TimeList() { this->deleteList(); }

int TimeList::length() { return len; }

void TimeList::insert(Time time1, PCB *p1) {
	lockCout
	Elem *tmp = new Elem(time1, p1);
	len++;
	if (!first) {
		first = last = tmp;
	}
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
		if (cur) {
			tmp->next = cur; cur->time = curTime - time1;
		}
		else last = tmp;
	}
	unlockCout
}

void TimeList::removeTimer() {
	if (!first) return;
	lockCout
	first->time--;
	Elem *old = 0;
	while (first && first->time == 0) {

		first->p->state = READY;
		first->p->unblockedByTime = 1;
		Scheduler::put(first->p);

		++(sem->value);

		old = first;
		first = first->next;
		len--;

		lockCout
		delete old;
		unlockCout
	}
	if (!first) last = 0;
	unlockCout
}

PCB* TimeList::removeAtFront() {
	if (!first) return 0;
	Elem *old = first;
	PCB *ret = old->p;
	first = first->next;
	if (!first) last = 0;
	else first->time += old->time;
	len--;

	lockCout
	delete old;
	unlockCout

	return ret;
}


void TimeList::ispis() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	printf("Pocinjem ispis TimeListe\n");
	Elem *tmp = first;
	Time curTime = 0;
	while (tmp) {
		curTime += tmp->time;
		cout << tmp->time << "[" << curTime << "] ";
		tmp = tmp->next;
	}
	cout << endl;
	unlock
}



