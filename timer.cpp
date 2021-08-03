#include <dos.h>

#include "timer.h"
#include "SCHEDULE.h"
#include "pcb.h"

volatile unsigned int lockFlag = 1;

unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int counter = defaultTimeSlice;
volatile int context_switch_on_demand = 0;

void interrupt timer(){	// prekidna rutina
	int dummy=0;
	if (!context_switch_on_demand) brojac--; 
	if (brojac == 0 || context_switch_on_demand) {
		if(lockFlag==1){ // dozvoljena promena konteksta
			context_switch_on_demand=0;
			asm {
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp;
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			running= getNextPCBToExecute();	// Scheduler

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			brojac = running->kvant;

			asm {
				mov sp, tsp   // restore sp
				mov ss, tss
				mov bp, tbp
			}
		}
		else context_switch_on_demand=1;

	} 
    
	// poziv stare prekidne rutine koja se 
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida	
	if(!context_switch_on_demand) asm int 60h;
		                                              
}


// postavlja novu prekidnu rutinu
void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0020h	
		mov word ptr oldTimerOFF, ax	

		mov word ptr es:0022h, seg timer	 //postavlja 
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu	
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
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
		sti
	}
}

