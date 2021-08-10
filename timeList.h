

// timeList

class KerSem;
class PCB;

class TimeList {
public:
	// stare Vezbe5.pdf zad 6. Jun 2010
	struct Elem {
		Time time;
		PCB *p;
		Elem *next;
		Elem (Time time1, PCB *p1, Elem *next1 = 0) {
			time = time1; p = p1; next = next1;
		}
		~Elem() { time = 0; p = 0; next = 0; }
	};
	Elem *first;
	KerSem *sem;

	TimeList(KerSem *sem1);
	~TimeList();

	int length();
	void deleteList();

	void insert(Time time1, PCB *p1);
	void remove();

};