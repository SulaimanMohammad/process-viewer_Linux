#ifndef _CAPTEUR_H_
#define _CAPTEUR_H_

#include <stdlib.h>
#include <stdio.h>
#include <proc/readproc.h>
struct state
{
    char name[1000];
    char statue;
    int parentID;
    long unsigned int utime;
    long int cutime;
    long unsigned int stime;
    long int cstime;
    long int priority;
    long int  nice;
    long long unsigned int starttime;
    long unsigned int vsize; // virtual memory size in bytes
    long unsigned int rss; //Resident  Set  Size in bytes
    long unsigned int pTime;
    float cpu_total_time;
    };

typedef struct machine_info
{
  size_t size_memorie;
  char *name;

}machine_info_t;

long unsigned int get_uptime();

void process_value(long pid);

#endif
