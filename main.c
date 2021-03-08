#include <stdio.h>
#include <stdlib.h>
#include "capteur.h"
#include "server.h"
int main(int argc, char **argv)
{

  int pid;
  sscanf(argv[1], "%d", &pid);

  printf("=============================================+==================\n");
  printf("|   PID   |   Nom   |   S   |   Time   |   CPU   |   MEM   |     \n");
  printf("================================================================\n");

  process_value(pid);
  return 0;
}
