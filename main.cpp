#include "System.h"

int userMain(int argc, char *argv[]);

int main(int argc, char *argv[]) {
	System::initialize();
	int ret = userMain(argc, argv);
	System::restore();
	return ret;
}


