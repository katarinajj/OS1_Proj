/**
 * USER7.CPP
 *
 * Creates the maximum amount of threads possible within the system memory
 * and frees them, hoping nothing will go wrong.
 */
#include "asystem.h"
#include "thread.h"
#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

int syncPrint(const char *format, ...)
{
	int res;
	va_list args;
	lockCout
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	unlockCout
	return res;
}

class OveruseThread : public Thread {
    public:
        OveruseThread() : Thread(1024, 20) {}
        virtual void run() {
            syncPrint("This should not happen.\n");
        }
        ~OveruseThread() {
            waitToComplete();
        }
};

const int n = 1000;

void tick() {}

Thread* threads[n];

int userMain(int argc, char* argv[]) {
    (void) argc;
    (void) argv;
    unsigned i = 0;
    for (; i < n; ++i) {
        syncPrint("Creating %d\n", i);
        lockCout
        threads[i] = new OveruseThread();
        unlockCout
        if (threads[i] == 0 || threads[i]->getId() == -1) {
            syncPrint("Failed at index %d\n", i);
            if (threads[i] != 0) {
                lockCout
                delete threads[i];
                unlockCout
            }
            break;
        }
    }
    syncPrint("OVDE SAM\n");
    for (unsigned j = 0; j < i; ++j) {
    	lockCout
        delete threads[j];
    	unlockCout
        //syncPrint("Zavrsio %d\n", j);
    }
    syncPrint("Done\n");
    return 0;
}
