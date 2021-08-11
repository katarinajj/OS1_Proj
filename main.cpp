#include "asystem.h"

int userMain(int argc, char* argv[]);

int main(int argc, char *argv[]){

	inic();

	//Kernel::allocateAll();

	int ret = userMain(argc, argv);

	restore();

	Kernel::deleteAll();

	return ret;
}
