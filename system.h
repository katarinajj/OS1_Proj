#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stdio.h>

class PCB;
class List;

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

extern PCB* mainPCB;
extern PCB* idlePCB;
extern volatile PCB* running;
extern List* allPCBs;

// deklaracije funkcija

void myPrintf();

void tick();
void inic();
void restore();
void allocateAll();
void deleteAll();
void interrupt timer();


#endif /* SYSTEM_H_ */
