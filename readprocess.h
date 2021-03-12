#ifndef _READPROCESS_H_
#define _READPROCESS_H_
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <ctype.h>
#include <dirent.h>
#include <math.h>
#include <stdbool.h>
#include <sys/syscall.h>


/*
||===================================================================||
||-----------------------------STRUCTURES ----------------------------||
||===================================================================||
*/

 struct state{
    //read from stat file
    int pid;
    int  ppid;
    char name[1000];
    char statue;
   // int parentID;/
    long unsigned int utime;
    long int cutime;
    long unsigned int stime;
    long int cstime;
    long int priority;
    long int  nice;
    long long unsigned int starttime;
    long unsigned int vsize; // virtual memory size in bytes
    long int rss; //Resident  Set  Size in bytes
    //calculated
    //long unsigned int pTime;
    long unsigned int process_time;
    float cpu_total_time;
    float mem;
    long unsigned int shared_m_size;

    };

  struct cpu_info {
  long long unsigned int user;
  long long unsigned int nice;
  long long unsigned int system;
  long long unsigned int idle;
  long long unsigned int iowait;
  long long unsigned int irq ;
  long long unsigned int softirq;
  long long unsigned int steal;
  long long unsigned int guest, guestnice ;
  long long unsigned int total_usage;
  long unsigned int total_memkb;

   };

// void get_sharedmemory_size(struct state *proc, int pid);
 void comput_process_time(struct state  *proc, int pid);
 void get_sharedmemory_size(struct state *proc, int pid);
 bool process_isPerent(int pid);
 long unsigned int get_total_mem();
 char* find_usrname(int pid);
 long unsigned int get_uptime();
 void read_statFile (struct state  *proc, int pid);
 void get_total_cpu_usage(struct cpu_info *cpud);
 void get_process_cpu_usage(struct cpu_info *cpud , struct state  *proc, int pid);
// bool process_isPerent(int pid);
 //void comput_process_time(struct state  *proc, int pid);


#endif
