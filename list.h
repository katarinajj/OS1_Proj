#ifndef LIST_H_
#define LIST_H_

class PCB;

class List {

public:
	struct Elem {
		void *p;
		Elem *next;
		Elem(void *p1, Elem *next1 = 0) {
			p = p1; next = next1;
		}
		~Elem() { p = 0; next = 0; }
	};

	Elem *first, *last, *cur, *prev;
	int len;

	List();
	~List();

	int length();
	void deleteList();

	// QUEUE functions
	void insertAtEnd(void *q);
	void* removeAtFront();

	// PCB functions
	void removePCB(PCB *p1);

	// iterating functions
	void onFirst();
	void onNext();
	int hasCur();
	void* getCur();

private:

};


#endif /* LIST_H_ */
