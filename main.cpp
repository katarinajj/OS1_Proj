#include "asystem.h"
#include "thread.h"

int userMain(int argc, char* argv[]);

class UserMainThread : public Thread {
public:
	UserMainThread(int userArgc1, char **userArgv1) : Thread(defaultStackSize, 0) {
		this->userArgc = userArgc1;
		this->userArgv = userArgv1;
	}
	~UserMainThread() { waitToComplete(); }
	Thread* clone() const { return new UserMainThread(this->userArgc, this->userArgv); }

protected:
	void run() {
		userMain(this->userArgc, this->userArgv);
	}

private:
	int userArgc;
	char** userArgv;
};

int main(int argc, char *argv[]){

	inic();

	// pokretanje userMain niti
	Thread *usrMainThread = new UserMainThread(argc, argv);
	usrMainThread->start();
	delete usrMainThread;
	// restauracija sistema

	Kernel::deleteAll();

	restore();

	return 0;
}
