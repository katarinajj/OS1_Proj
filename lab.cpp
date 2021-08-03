//pretpostavljeni memorijski model: huge

#include <iostream.h>
#include <dos.h>
#include "schedule.h"

// Zabranjuje prekide
#define lock asm cli

#define lockCout lockFLag = 0;

#define unlockCout lockFlag = 1;\
		if (zahtevana_promena_konteksta) {\
			dispatch();\
		}

// Dozvoljava prekide
#define unlock asm sti

class PCB{
public:
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned zavrsio;
	int kvant;
};

PCB *p[3];
volatile PCB* running; 

volatile int nextThread = 2;
PCB* getNextPCBToExecute(){
	if (nextThread == 1)
		nextThread = 2;
	else nextThread = 1;
	if (p[nextThread]->zavrsio){
		if (nextThread == 1)
			nextThread = 2;
		else nextThread = 1;
		if (p[nextThread]->zavrsio)
			nextThread = 0;
	}
	return p[nextThread];
}

volatile unsigned lockFlag = 1;
//pomocne promenljive za prekid tajmera
unsigned tbp;
unsigned tsp;
unsigned tss;

volatile int brojac = 20;
volatile int zahtevana_promena_konteksta = 0;

// po ulasku u timer() I bit je sigurno 0
void interrupt timer(){	// prekidna rutina
	int dummy = 1;
	if (!zahtevana_promena_konteksta) brojac--; 
	if (brojac == 0 || zahtevana_promena_konteksta) {
		if (lockFlag) {
			zahtevana_promena_konteksta = 0;
			asm {   // cuva sp i bp
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
			}

			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			// ispis unutar prekidne rutine
			lockFlag = 0;
			cout << "Promena konteksta! Brojac = " << brojac << endl;
			// ako neko vec vrsi ispis, lockFlag je vec na 0 i zato se nece ni poceti promena
			// konteksta, pa samim tim se ne moze desiti ni ovaj ispis
			asm cli; // nekad se prekidi omoguce unutar cout<<...
			lockFlag = 1;

			//running= getNextPCBToExecute();	// Scheduler
			if (!running->zavrsio) Scheduler::put((PCB*) running);
			running = Scheduler::get();

			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;

			brojac = running->kvant;

			asm {   // restore sp
					mov sp, tsp
					mov ss, tss
					mov bp, tbp
			}
		}
		else zahtevana_promena_konteksta = 1;

	} 
    
	 // poziv stare prekidne rutine koja se nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida	
    if(!zahtevana_promena_konteksta) asm int 60h;

	// zahtevana_promena_konteksta = 0; - sad se menja u zavisnosti of lockFlaga
}

void dispatch(){ // sinhrona promena konteksta 
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

unsigned oldTimerOFF, oldTimerSEG; // stara prekidna rutina

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

void exitThread(){
	running->zavrsio = 1;
	dispatch();
}  

void a(){
	for (int i =0; i < 30; ++i) {
		lockFlag = 0;
		cout<<"u a() i = "<<i<<endl;
		lockFlag = 1;
		if (zahtevana_promena_konteksta) {
			dispatch();
		}
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		lockFlag = 0;
		cout<<"u b() i = "<<i<<endl;
		lockFlag = 1;
		if (zahtevana_promena_konteksta) {
			dispatch();
		}
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}


void createProcess(PCB *newPCB, void (*body)()){
	unsigned* st1 = new unsigned[1024];

	st1[1023] =0x200;//setovan I fleg u
                      // pocetnom PSW-u za nit
#ifndef BCC_BLOCK_IGNORE
	st1[1022] = FP_SEG(body);
	st1[1021] = FP_OFF(body);

	newPCB->sp = FP_OFF(st1+1012); //svi sacuvani registri pri ulasku u interrupt rutinu
	newPCB->ss = FP_SEG(st1+1012);
	newPCB->bp = FP_OFF(st1+1012); // pocetni bp treba da pokazuje na poziciju
	// na kojoj se cuva stara vrednost bp
#endif
	newPCB->zavrsio = 0;
}



void doSomething(){
	lock
	p[1] = new PCB();
	createProcess(p[1],a);
	cout<<"napravio a"<<endl;
	p[1]->kvant = 40;
	Scheduler::put(p[1]);

	p[2] = new PCB();
	createProcess(p[2],b);
	cout<<"napravio b"<<endl;
	p[2]->kvant = 20;
	Scheduler::put(p[2]);

	p[0] = new PCB();
	p[0]->kvant = 20;
	p[0]->zavrsio = 0;

	running = p[0];
	unlock

#ifndef BCC_BLOCK_IGNORE
	for (int i = 0; i < 30; ++i) {
  	lock
  	cout<<"main "<<i<<endl;
  	unlock

	for (int j = 0; j< 30000; ++j)
		for (int k = 0; k < 30000; ++k);
	}
#endif
	cout<<"Happy End"<<endl;
}

int main(){
	
  inic();

  doSomething();
  
  restore();
 
  return 0;
}
