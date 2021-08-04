#include "pcb.h"


void List::deleteList() {
	while (first) {
		Elem *old = first;
		first = first->next;
		delete old;
	}
	first = last = 0;
	len = 0;
}

void List::copyList(const List &l) {
	len = l.len;
	if (l.first == 0 && l.last == 0) {
		first = last = 0;
	}
	else {
		first = last = new Elem(l.first->p);
		for (Elem *tmp = l.first->next; tmp; tmp = tmp->next) {
			last->next = new Elem(tmp->p);
			last = last->next;
		}
	}
}

List::List() {
	first = last = 0;
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
	Elem *tmp = new Elem(p);
	len++;
	last = (!first ? first : last->next) = tmp;
}

void List::removePCB(PCB *p1) {
	Elem *tmp = first, *prev = 0;
	if (tmp && ((PCB*)(tmp->p))->getId() == p1->getId()) {
		first = first->next;
		delete tmp;
		return;
	}
	for (; tmp && ((PCB*)(tmp->p))->getId() != p1->getId(); tmp = tmp->next)
		prev = tmp;

	if (!tmp) return; // nije ni bio u listi
	prev->next = tmp->next;
	delete tmp;
}

PCB* List::getPCBbyId(ID id) {
	Elem *tmp = first;
	for (; tmp && ((PCB*)(tmp->p))->getId() != id; tmp = tmp->next);

	if (!tmp) return 0; // nije ni bio u listi
	else return (PCB*)tmp->p;
}



