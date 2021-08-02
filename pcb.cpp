#include <iostream>
#include <dos.h>

#include "pcb.h"
#include "schedule.h"

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*body)()) {
	unsigned long numOfIndex = stackSize / sizeof(unsigned);
	unsigned* st1 = new unsigned[numOfIndex];
	
	st1[numOfIndex - 1] = 0x200;//setovan I fleg u pocetnom PSW-u za nit
	st1[numOfIndex - 2] = FP_SEG(body);
	st1[numOfIndex - 3] = FP_OFF(body);
	
	this->ss = FP_SEG(st1 + numOfIndex - 12);
	this->sp = FP_OFF(st1 + numOfIndex - 12);  //svi sacuvani reg pri ulasku u interrupt rutinu
    this->bp = FP_OFF(st1 + numOfIndex - 12); 
   
	//this->state = s;
	
}
