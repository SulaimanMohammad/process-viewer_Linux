#include <stdio.h>
#include <stdlib.h>
#include "capteur.h"
#include "server.h"
#include "client.h"


#include "readprocess.h"

void prise_client()
{
  my_client();
}

void prise_serveur()
{
  serveur();
}
void main(int argc, char **argv)
{
//int pids[4194303]={0};//there is no pid is zero
  int option =0;

  while ((option = getopt(argc, argv, "sc")) != -1)
    {
      switch (option)
      {
        /*client*/
        case 'c':
          prise_client();
          break;
          /*serveur*/
        case 's':
          prise_serveur();
          break;

        default:
          printf("Erreur de choix\n");
          break;
      }
    }
}
