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
		~Elem() { 
			p = 0; next = 0;
		}
	};
	
	Elem *first, *last;
	int len;
	
	List();
	//List(const List &l);
	~List();
	
	//List& operator=(const List &l);
	
	int length();
	
	void add(PCB *p);
	void remove(PCB *p);
	void deleteList();
	
private:
	void copyList(const List &l);
};

#endif