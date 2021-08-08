#include "system.h"

int userMain(int argc, char *arvg[]);

int main(int argc, char *argv[]){

  inic();

  allocateAll();

  int ret = userMain(argc, argv);

  restore();

  deleteAll();

  return ret;
}
