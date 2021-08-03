#ifndef TIMER_H_
#define TIMER_H_

#include "pcb.h"

// MAKROI
// Zabranjuje prekide
#define lock asm cli

#define lockCout lockFlag = 0;

#define unlockCout lockFlag = 1;\
		if (context_switch_on_demand) {\
			dispatch();\
		}

// Dozvoljava prekide
#define unlock asm sti
// -----------------

extern volatile unsigned lockFlag;

extern unsigned tbp;
extern unsigned tsp;
extern unsigned tss;

extern volatile int counter;
extern volatile int context_switch_on_demand;

void inic();
void restore();
void interrupt timer();


#endif /* TIMER_H_ */
