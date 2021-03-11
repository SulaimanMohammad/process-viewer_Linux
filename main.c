#include <stdio.h>
#include <stdlib.h>
#include "capteur.h"
#include "server.h"
int main(int argc, char **argv)
{

  proc_info_t *tab = capteur();
  affiche_mytop(tab);
  free_proc(tab);
	  
  return 0;
}
