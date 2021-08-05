#ifndef TIMER_H_
#define TIMER_H_

#include "pcb.h"

extern unsigned tbp;
extern unsigned tsp;
extern unsigned tss;

extern volatile int counter;

void tick();
void inic();
void restore();
void interrupt timer();


#endif /* TIMER_H_ */
