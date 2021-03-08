#include <stdio.h>
#include <stdlib.h>
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
#include "capteur.h"

long unsigned int get_uptime()
{
  char buff[128];
  char *p;
  int fd;
  long unsigned int uptime;

 if ((fd = open("/proc/uptime", 0)) != -1)
 {
   if (read(fd, buff, sizeof(buff)) > 0)
   {
     uptime = strtoul(buff, &p, 10);

   }
   close(fd);
 }
return (uptime);
}



void process_value(long pid)
{  char filename[1000];
   sprintf(filename, "/proc/%d/stat", pid);
   FILE *f = fopen(filename, "r");
  // long long process_start_time_since_boot;
   struct state* proc;
  // bzero(proc, sizeof(struct state));
   proc = malloc(sizeof *proc);

   fscanf(f, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
               "%lu %ld %ld %ld %ld %*d %*d %llu %lu %ld",
               proc->name,&proc->statue,
               &proc->utime,&proc->stime,&proc->cutime,
               &proc->cstime,&proc->priority, &proc->nice,
               &proc->starttime, &proc->vsize, &proc->rss);
  fclose(f);

 long unsigned int process_time= proc->utime + proc->stime; //total time spent for the process
//process_time=process_time+proc->cutime+cstime //with children
 proc->pTime = get_uptime() -(proc->starttime/sysconf (_SC_CLK_TCK));
 proc->cpu_total_time =100*((process_time /sysconf (_SC_CLK_TCK))/proc->pTime);


 printf("%8d",pid);
 printf("%12s",proc->name);
 printf("%5c",proc->statue);
 printf("%11lu",proc->pTime);
 printf("%14f\n", proc->cpu_total_time);
 free(proc);

}
  
