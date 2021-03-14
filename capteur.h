#ifndef _CAPTEUR_H_
#define _CAPTEUR_H_

#include <stdlib.h>
#include <stdio.h>
#include <proc/readproc.h>

#define PROC_FLAGS (PROC_FILLMEM | PROC_FILLCOM | PROC_FILLENV | PROC_FILLUSR |PROC_FILLGRP | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_EDITCMDLCVT)

#define NB_MAX_PROCESS 200
typedef struct proc_info_s
{
	size_t nproc;
	size_t nb_process;
	proc_t proc_info[NB_MAX_PROCESS];

	size_t size_memorie;
	char name[200];
}proc_info_t;

proc_info_t *capteur();

void affiche_mytop(proc_info_t *m);

void free_proc(proc_info_t *m);

#endif
