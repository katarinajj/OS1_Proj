#include "pcb.h"


void List::deleteList() {
	while (first) {
		Elem *old = first;
		first = first->next;
		lockCout
		delete old;
		unlockCout
	}
	first = last = cur = prev = 0;
	len = 0;
}

List::List() {
	first = last = cur = prev = 0;
	len = 0;
}

List::~List() { deleteList(); }

int List::length() { return len; }

void List::insertAtEnd(void *q) {
	lockCout
	Elem *tmp = new Elem(q);
	unlockCout
	len++;
	last = (!first ? first : last->next) = tmp;
}

void* List::removeAtFront() {
	if (!first) return 0;
	Elem *old = first;
	void *ret = old->p;
	first = first->next;
	if (!first) last = 0;
	len--;
	lockCout
	delete old;
	unlockCout
	return ret;
}

void List::removePCB(PCB *p1) {
	Elem *tmp = first, *prev1 = 0;
	if (tmp && ((PCB*)(tmp->p))->getId() == p1->getId()) {
		first = first->next;
		len--;
		lockCout
		delete tmp;
		unlockCout
		return;
	}
	for (; tmp && ((PCB*)(tmp->p))->getId() != p1->getId(); tmp = tmp->next)
		prev1 = tmp;

	if (!tmp) return; // nije ni bio u listi
	prev1->next = tmp->next;
	len--;
	lockCout
	delete tmp;
	unlockCout
}

// kod po ugledu na vezbe iz OOP1

void List::onFirst() { cur = first; prev = 0; }

void List::onNext() {
	prev = cur;
	if (cur) cur = cur->next;
}

int List::hasCur() { return cur != 0; }

void* List::getCur() {
	if (!cur) return 0;
	return cur->p;
}




