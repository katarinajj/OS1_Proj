#include "thread.h"

/*
	Test: Niti maxStack velicine
*/

#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>

int syncPrintf(const char *format, ...)
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

const int n = 9;

void tick(){}

class TestThread : public Thread
{
public:

	TestThread(): Thread(65536,2) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{

	int buffer=2;

	for(int i=0;i<32000;i++)
	{
		buffer = 4096/2048;
		for (int j = 0; j < 1024; j++)
		{
			buffer = buffer*2;
			if(buffer%2)
				buffer = 2;
		}
	}

}


int userMain(int argc, char** argv)
{
	syncPrintf("Test starts: %d threads.\n",n);
	syncPrintf("Noviji kod.\n");
	int i;
	TestThread threads[n];
	Thread *th = 0;
	for(i=0;i<n;i++)
	{
		threads[i].start();
	}
	for(i=0;i<n;i++)
	{
		threads[i].waitToComplete();
		th = Thread::getThreadById(i+3);
		if (!th) syncPrintf("greskaaa main\n");
		else syncPrintf("%d. Done!\n", th->getId());
	}
	syncPrintf("Test ends.\n");
	return 0;
}


