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

	Elem *first, *last;
	int len;

	List();
	//List(const List &l);
	~List();

	//List& operator=(const List &l);

	int length();

	void addPCB(PCB *p);
	void removePCB(PCB *p);
	void deleteList();

	PCB* getPCBbyId(ID id);

private:
	void copyList(const List &l);
};




#endif /* LIST_H_ */
