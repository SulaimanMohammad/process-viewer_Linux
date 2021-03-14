#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <sys/sysinfo.h>
#include <proc/readproc.h>
#include "capteur.h"


proc_info_t *capteur()
{
	int cmpt=0;

	PROCTAB *tab_proc = openproc(PROC_FLAGS);
	
	proc_t **info = readproctab(PROC_FLAGS);


	closeproc(tab_proc);

	proc_info_t *machine = malloc(sizeof(proc_info_t));


	while(info[cmpt]!=NULL)
	{
		cmpt++;
	}
	machine->nb_process = cmpt;

	int con=gethostname(machine->name,200);
	if(con<0)
	{	
		perror("gethostname");
	}

	machine->nproc =get_nprocs();

	if(cmpt)
	{
		machine->size_memorie= machine->proc_info[0].vsize;
	}

	return machine;
}

void affiche_mytop(proc_info_t *m)
{
	if(!m)
	{
		exit(1);
	}

	printf("Process:  %ld\n", m->nb_process);
	printf("Machine: %s\n",m->name);
	printf("Process; %ld\n", m->nb_process);
	printf("Memory: %ld\n", m->nb_process);
}


