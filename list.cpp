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

void List::copyList(const List &l) {
	len = l.len;
	if (l.first == 0 && l.last == 0) {
		first = last = cur = prev = 0;
	}
	else {
		first = last = new Elem(l.first->p);
		cur = l.cur;
		prev = l.prev;
		for (Elem *tmp = l.first->next; tmp; tmp = tmp->next) {
			lockCout
			last->next = new Elem(tmp->p);
			unlockCout
			last = last->next;
		}
	}
}

List::List() {
	first = last = cur = prev = 0;
	len = 0;
}

/*
List::List(const List &l) {
	copyList(l);
}
*/

List::~List() {
	deleteList();
}

/*
List& List::operator=(const List &l) {
	if (this != &l) {
		deleteList();
		copyList(l);
	}
	return *this;
}*/

int List::length() {
	return len;
}

void List::addPCB(PCB *p) {
	lockCout
	Elem *tmp = new Elem(p);
	unlockCout
	len++;
	last = (!first ? first : last->next) = tmp;
}

void List::removePCB(PCB *p1) {
	Elem *tmp = first, *prev1 = 0;
	if (tmp && ((PCB*)(tmp->p))->getId() == p1->getId()) {
		first = first->next;
		lockCout
		delete tmp;
		unlockCout
		return;
	}
	for (; tmp && ((PCB*)(tmp->p))->getId() != p1->getId(); tmp = tmp->next)
		prev1 = tmp;

	if (!tmp) return; // nije ni bio u listi
	prev1->next = tmp->next;
	delete tmp;
}

void List::onFirst() {
	cur = first;
	prev = 0;
}

void List::onNext() {
	prev = cur;
	if (cur) cur = cur->next;
}

int List::hasCur() {
	return cur != 0;
}

void* List::getCur() {
	if (!cur) return 0;
	return cur->p;
}

void List::removeCur() {
	if (!cur) return;
	Elem *old = cur;
	cur = cur->next;
	(!prev ? first : prev->next) = cur;
	if (!cur) last = prev;
	lockCout
	delete old;
	unlockCout
}


