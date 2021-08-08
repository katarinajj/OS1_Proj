#include <iostream.h>
#include "pcb.h"

volatile int lockFlag = 0;
volatile unsigned context_switch_on_demand = 0;
volatile int counter = defaultTimeSlice;

unsigned tbp;
unsigned tsp;
unsigned tss;

PCB* mainPCB = 0;
PCB* idlePCB = 0;
volatile PCB* running = 0;
List* allPCBs = 0;

void myPrintf() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	printf("GRESKA\n");
	unlock
}

void allocateAll() {
	lockCout

	mainPCB = new PCB();
	idlePCB = new PCB(defaultStackSize, 1, 0, idleBody);
	running = mainPCB;
	allPCBs = new List();

	unlockCout
}


void interrupt timer(){

	if (!context_switch_on_demand) { asm int 60h; tick(); }
	if (!context_switch_on_demand && counter > 0) { counter--; }

	if ((counter == 0 && running->timeSlice != 0) || context_switch_on_demand) {
		if (lockFlag == 0) {
			context_switch_on_demand = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			/* ---------- ispis unutar prekidne rutine
			lockFlag = 0;
			cout << "Promena konteksta! Brojac = " << counter << endl;
			asm cli; // nekad se prekidi omoguce unutar cout<<...
			lockFlag = 1;
			*/

			if (running->state == READY) Scheduler::put((PCB*)running);
			running = Scheduler::get();
			if (running == 0) running = idlePCB;

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			counter = running->timeSlice;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else context_switch_on_demand = 1;
	}
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void inic(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	asm{
		push es
		push ax

		mov ax,0   // inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h // pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 // postavlja
		mov word ptr es:0020h, offset timer  // novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu na int 60h
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
	}
	unlock
}

void restore(){
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	asm {
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
	}
	unlock
}

void dispatch() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	context_switch_on_demand = 1;
	timer();
	unlock
}

void deleteAll() {
	lockCout
	if (allPCBs) delete allPCBs;
	if (mainPCB) delete mainPCB;
	if (idlePCB) delete idlePCB;
	unlockCout
}



