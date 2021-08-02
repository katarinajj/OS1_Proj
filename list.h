#ifndef _list_h_
#define _list_h_

#include "pcb.h"

class List {

public:
	struct Elem {
		PCB *p;
		Elem *next;
		Elem(PCB *p1, Elem *next1 = 0) {
			p = p1; next = next1;
		}
	};
	
	Elem *first, *last;
	int len;
	
	List();
	List(const List &l);
	~List();
	
	List& operator=(const List &l);
	
	int length();
	
	List& add(PCB *p);
	
private:
	void deleteList();
	void copyList(const List &l);
};

#endif