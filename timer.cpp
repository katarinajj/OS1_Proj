#include <iostream.h>

#include "timer.h"

volatile unsigned lockFlag = 1;

unsigned tbp;
unsigned tsp;
unsigned tss;

volatile int counter = 20;
volatile int context_switch_on_demand = 0;

// po ulasku u timer() I bit je sigurno 0
void interrupt timer(){	// prekidna rutina

	if (!context_switch_on_demand) { counter--; asm int 60h; }

	if ((counter == 0 && PCB::running->timeSlice != 0) || context_switch_on_demand) {
		if (lockFlag) {
			context_switch_on_demand = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			// ---------- ispis unutar prekidne rutine
			lockFlag = 0;
			cout << "Promena konteksta! Brojac = " << counter << endl;
			// ako neko vec vrsi ispis, lockFlag je vec na 0 i zato se nece ni poceti promena
			// konteksta, pa samim tim se ne moze desiti ni ovaj ispis
			asm cli; // nekad se prekidi omoguce unutar cout<<...
			lockFlag = 1;


			if (PCB::running->state != TERMINATED) Scheduler::put((PCB*) PCB::running);
			PCB::running = Scheduler::get();

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			counter = PCB::running->timeSlice;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else context_switch_on_demand = 1;
	}
	 // poziv stare prekidne rutine koja se nalazila na 08h, a sad je na 60h poziva se samo kada nije zahtevana
	 // promena konteksta – tako se da se stara rutina poziva samo kada je stvarno doslo do prekida
    //if (!context_switch_on_demand || (!counter && !lockFlag)) asm int 60h;
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

// postavlja novu prekidnu rutinu
void inic(){
	lock
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
	lock
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
	lock
	context_switch_on_demand = 1;
	timer();
	unlock
}
