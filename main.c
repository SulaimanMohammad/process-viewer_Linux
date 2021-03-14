#include <stdio.h>
#include <stdlib.h>
#include "capteur.h"
#include "server.h"
#include "client.h"


int main(int argc, char **argv)
{

  proc_info_t *tab = capteur();
  affiche_mytop(tab);


  return 0;
}
