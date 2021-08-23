#include "kerSem.h"

volatile int lockFlag = 0;
volatile unsigned context_switch_on_demand = 0;
volatile int counter = defaultTimeSlice;
volatile unsigned ticks = 0;
volatile int numOfUnfinishedPCBs = 0;
volatile unsigned idleLoopConst = 1;

unsigned tbp;
unsigned tsp;
unsigned tss;

PCB* Kernel::mainPCB = new PCB();
PCB* Kernel::idlePCB = new PCB(512, 1, 0, idleBody);
volatile PCB* Kernel::running = mainPCB;
List* Kernel::allPCBs = new List();
List* Kernel::allKernelSems = new List();
IVTEntry* Kernel::ivtEntries[numOfEntries];

void myPrintf() {
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	printf("GRESKA\n");
	unlock
}

void interrupt timer(){

	if (!context_switch_on_demand) { asm int 60h; tick(); }
	if (!context_switch_on_demand && counter > 0) { counter--; }

	// dodatak za semafore

	if (lockFlag == 0 && !context_switch_on_demand) {
		++ticks;
		for (Kernel::allKernelSems->onFirst(); Kernel::allKernelSems->hasCur(); Kernel::allKernelSems->onNext()) {
			((KernelSem*)(Kernel::allKernelSems->getCur()))->waitingPCBs->removeTimer();
		}
		ticks = 0;
	}
	else if (lockFlag > 0 && !context_switch_on_demand) { ++ticks; }

	if ((counter == 0 && Kernel::running->timeSlice != 0) || context_switch_on_demand) {
		if (lockFlag == 0) {
			context_switch_on_demand = 0;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			Kernel::running->sp = tsp;
			Kernel::running->ss = tss;
			Kernel::running->bp = tbp;

			/* ---------- ispis unutar prekidne rutine
			lockFlag = 1;
			cout << "Promena konteksta! Brojac = " << counter << endl;
			asm cli; // nekad se prekidi omoguce unutar cout<<...
			lockFlag = 0;
			*/

			if (Kernel::running->state == READY) Scheduler::put((PCB*)Kernel::running);

			Kernel::running = Scheduler::get();


			if (Kernel::running == 0 && numOfUnfinishedPCBs == 0) Kernel::running = Kernel::mainPCB;
			else if (Kernel::running == 0) Kernel::running = Kernel::idlePCB;

			tsp = Kernel::running->sp;
			tss = Kernel::running->ss;
			tbp = Kernel::running->bp;

			counter = Kernel::running->timeSlice;

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
	//printf("U dispatch()\n");
	context_switch_on_demand = 1;
	timer();
	unlock
}

void Kernel::deleteAll() {
	lockCout

	if (numOfUnfinishedPCBs != 0) {
		printf("--- suspendujem main\n");
		Kernel::running->state = SUSPENDED;
		unlockCout
		dispatch();
	}
	else {
		unlockCout
	}
	printf("\nallPCBs len: %d\n", Kernel::allPCBs->len);
	printf("allKernelSems len: %d\n", Kernel::allKernelSems->len);
	printf("lockFlag: %d\n", lockFlag);

	if (Kernel::allPCBs) delete Kernel::allPCBs;
	if (Kernel::mainPCB) delete Kernel::mainPCB;
	if (Kernel::idlePCB) delete Kernel::idlePCB;
	if (Kernel::allKernelSems) delete Kernel::allKernelSems;

}



