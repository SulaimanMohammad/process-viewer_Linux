#ifndef _CAPTEUR_H_
#define _CAPTEUR_H_

#include <stdlib.h>
#include <stdio.h>
#include <proc/readproc.h>

#define PROC_FLAGS (PROC_FILLMEM | PROC_FILLCOM | PROC_FILLENV | PROC_FILLUSR |PROC_FILLGRP | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_EDITCMDLCVT)


typedef struct proc_info_s
{
	size_t n;
	proc_t **info;
}proc_info_t;

proc_info_t *capteur();

#endif
