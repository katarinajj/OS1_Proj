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
		~Elem() {
			p = 0; next = 0;
		}
	};

	Elem *first, *last, *cur, *prev;
	int len;

	List();
	//List(const List &l);
	~List();

	//List& operator=(const List &l);

	int length();
	void deleteList();

	// PCB functions
	void addPCB(PCB *p); // dodaje PCB na kraj liste
	void removePCB(PCB *p);

	// iterating functions
	void onFirst();
	void onNext();
	int hasCur();
	void* getCur();
	void removeCur();

private:
	void copyList(const List &l);
};




#endif /* LIST_H_ */
