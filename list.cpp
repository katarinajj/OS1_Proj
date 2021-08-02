#include "list.h"


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
	if (l.first = 0 && l.last = 0) {
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

List::List(const List &l) {
	copyList(l);
}

List::~List() {
	deleteList();
}

List& List::operator=(const List &l) {
	if (this != &l) {
		deleteList();
		copyList(l);
	}
	return *this;
}

int List::length() {
	return len;
}

List& List::add(PCB *p) {
	Elem *tmp = new Elem(p);
	len++;
	last = (!first ? first : last->next) = tmp;
	return *this;
}