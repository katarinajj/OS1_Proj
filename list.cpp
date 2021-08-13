#include "pcb.h"


void List::deleteList() {
	lockCout
	Elem *old = first;
	while (first) {
		old = first;
		first = first->next;
		lockCout
		delete old;
		unlockCout
	}
	first = last = cur = prev = 0;
	len = 0;
	unlockCout
}


List::List() {
	lockCout
	first = last = cur = prev = 0;
	len = 0;
	unlockCout
}

List::~List() { deleteList(); }

int List::length() { return len; }

void List::insertAtEnd(void *q) {
	if (!q) {
		cout << "Insertujes null u listu\n";
		return;
	}

	lockCout

	lockCout
	Elem *tmp = new Elem(q);
	if (tmp == 0) {
		cout << "Nemam memorije u listi";
		unlockCout;
		unlockCout;
		return;
	}
	unlockCout

	len++;
	if (!first) first = tmp;
	else last->next = tmp;
	last = tmp;

	unlockCout
}

void* List::removeAtFront() {
	lockCout

	if (!first) { unlockCout; return 0; }
	Elem *old = first;
	void *ret = first->p;
	first = first->next;
	if (!first) last = 0;
	len--;

	lockCout
	delete old;
	unlockCout

	unlockCout
	return ret;
}

void List::removePCB(PCB *p1) { // obrisala sam okruzujuce lockove
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

void List::removeKernelSem(KernelSem *s) {
	Elem *tmp = first, *prev1 = 0;
	if (tmp && (KernelSem*)(tmp->p) == s) {
		first = first->next;
		len--;

		lockCout
		delete tmp;
		unlockCout

		return;
	}
	for (; tmp && (KernelSem*)(tmp->p) != s; tmp = tmp->next)
		prev1 = tmp;

	if (!tmp) return; // nije ni bio u listi
	prev1->next = tmp->next;
	len--;

	lockCout
	delete tmp;
	unlockCout
}

// kod po ugledu na vezbe iz OOP1

void List::onFirst() {
	lockCout
	cur = first; prev = 0;
	unlockCout
}

void List::onNext() {
	lockCout
	prev = cur;
	if (cur) cur = cur->next;
	unlockCout
}

int List::hasCur() {
	return cur != 0;
}

void* List::getCur() {
	if (!cur) { return 0; }
	return cur->p;
}

void List::ispis() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	printf("Pocinjem ispis liste: %d elem\n", this->len);
	Elem *tmp = first;
	while (tmp != 0) {
		printf("Element: %p\n", (KernelSem*)(tmp->p));
		tmp = tmp->next;
	}
	unlock
}




