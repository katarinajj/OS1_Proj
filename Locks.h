#ifndef LOCKS_H_
#define LOCKS_H_

// MAKROI
// hard lock - zabranjuje SVE maskirajuce prekide - dozvoljava gnezdjenje
#define lock asm { pushf; cli; }
#define unlock asm popf

// lock koji koristimo u kriticnim sekcijama da ne bi dolazilo do promene konteksta
#define lockCout lockFlag = 0;

#define unlockCout lockFlag = 1;\
		if (context_switch_on_demand) {\
			dispatch();\
		}

// -----------------

extern volatile unsigned lockFlag;



#endif /* LOCKS_H_ */
