/**
 * USER3.CPP
 *
 * Tests threads which simply echo the message passed a specified amount of
 * times in specified intervals.
 */
#include "thread.h"
#include "system.h"

#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

void dumbSleep(int delay) {

	for (int k = 0; k < 10; ++k) {
		for (int i = 0; i < 1000; ++i) {
		          for (int j = 0; j < delay; ++j);
		      }
	}

	/*
	for (int i = 0; i < 1000; ++i) {
			 for (int j = 0; j < delay; ++j);
	}*/

}

int syncPrint(const char *format, ...)
{
	int res;
	va_list args;
	asm { pushf; cli; }
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	asm popf;
	return res;
}

class EchoThread : public Thread {
    public:
        EchoThread(const char* message, unsigned times=1, unsigned delay=1) :
            Thread(1024, 1), message(message), times(times), delay(delay) {}
        virtual void run();
        ~EchoThread() {
            waitToComplete();
        }
    private:
        const char* message;
        unsigned times;
        unsigned delay;
};

void EchoThread::run() {
    //syncPrint("First message from thread %d\n", getId());
    for (unsigned i = 0; i < times; ++i) {
        syncPrint("%s from thread %d\n", message, getId());
        dumbSleep(delay * 1000);
    }
    //syncPrint("Last message from thread %d\n", getId());
}

void tick() {}

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    lockCout
    EchoThread** threads = new EchoThread*[256];
    unlockCout
    for (unsigned i = 0; i < 256; ++i) {
        lockCout
        threads[i] = new EchoThread("ECHO", i % 4, i % 10);
        unlockCout
        threads[i]->start();
        if (i % 20 == 0) {
            syncPrint("Dispatching main\n");
            dispatch();
        }
    }
    for (unsigned j = 0; j < 256; ++j) {
        // We cannot wait for a thread to complete while interrupts are locked!
        threads[j]->waitToComplete();
        lockCout
        delete threads[j];
        unlockCout
    }
    lockCout
    delete[] threads;
    unlockCout
    return 0;
}
