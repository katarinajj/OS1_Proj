#ifndef ASYSTEM_H_
#define ASYSTEM_H_

#include <stdio.h>

class PCB;
class List;
class KernelSem;

// hard lock - zabranjuje SVE maskirajuce prekide - dozvoljava gnezdjenje
#define lock asm { pushf; cli; }
#define unlock asm popf

// TODO: proveri ove lockove
// lock koji koristimo u kriticnim sekcijama da ne bi dolazilo do promene konteksta - dozvoljava gnezdjenje
#define lockCout ++lockFlag;

#define unlockCout if (lockFlag > 0) { --lockFlag; }\
				   else { myPrintf(); }\
				   if (lockFlag == 0 && context_switch_on_demand == 1) {\
					   dispatch();\
				   }

// deklaracije promenljivih

extern volatile int lockFlag; // inicijalno 0
extern volatile unsigned context_switch_on_demand;
extern volatile int counter;

extern unsigned tbp;
extern unsigned tsp;
extern unsigned tss;

class Kernel {
public:
	static PCB* mainPCB;
	static PCB* idlePCB;
	static volatile PCB* running;
	static List* allPCBs;
	static List* allKernelSems;

	static void deleteAll();
};


// deklaracije funkcija

void myPrintf();

void tick();
void inic();
void restore();
void interrupt timer();


#endif /* ASYSTEM_H_ */
