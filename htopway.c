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

struct state{
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

void main(int argc, char **argv) {
    int pid;
    sscanf(argv[1], "%d", &pid);
   // printf("pid = %d\t", pid);

  printf("=============================================+==================\n");
  printf("|   PID   |   Nom   |   S   |   Time   |   CPU   |   MEM   |     \n");
  printf("================================================================\n");

  process_value(pid);

  //bring all the process 
  /*DIR* proc = opendir("/proc");
  struct dirent* ent;
  long tgid;

  if(proc == NULL) {
    perror("opendir(/proc)");
    return 1;
  }

 while(ent = readdir(proc)) 
 {
    if(!isdigit(*ent->d_name))
        continue;

    tgid = strtol(ent->d_name, NULL, 10);
   //call the function 
 }
 closedir(proc);
*/
}


// bzero(proc, sizeof(struct state)); 
    /* The bzero() function erases the data in the n bytes of the memory
       starting at the location pointed to by s, by writing zeros (bytes
       containing '\0') to that area.*/
