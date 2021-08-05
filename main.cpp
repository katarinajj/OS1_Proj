#include <iostream.h>

#include "timer.h"

int userMain(int argc, char *arvg[]);

int main(int argc, char *argv[]){

  inic();

  int ret = userMain(argc, argv);

  restore();

  if (PCB::allPCBs) delete PCB::allPCBs;

  return ret;
}
