#include "asystem.h"
#include "list.h"

int userMain(int argc, char* argv[]);

int main(int argc, char *argv[]){

	inic();

	//Kernel::allocateAll();

	int ret = userMain(argc, argv);

	restore();

	printf("allPCBs len: %d\n", Kernel::allPCBs->length());
	printf("allKernelSems len: %d\n", Kernel::allKernelSems->length());
	printf("lockFlag: %d\n", lockFlag);

	Kernel::deleteAll();

	return ret;
}
