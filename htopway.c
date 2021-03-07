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

long unsigned int total_memkb;
struct state{
    //read from stat file
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
    long int rss; //Resident  Set  Size in bytes
    //calculated 
    long unsigned int pTime;
    float cpu_total_time;
    float mem;
    };


 long unsigned int get_total_mem()
 {
   char  line[100];
  // long unsigned int p;
   int uid;
   const char s[2] = ":";
      FILE* statusf  = fopen("/proc/meminfo", "r");
   if(!statusf)
     return 1;

     while(fgets(line,100, statusf))
      {
        if(strncmp(line, "MemTotal:", 9) == 0)
         {
          sscanf(line,"%*s %ld",&total_memkb );//reads formatted input from a string.
         // printf("%ld\n", total_memkb);
        break;
        }
      }
 fclose(statusf);
 return total_memkb;
 }

 char find_usrname(int pid)
 {

   char fuid[1000],fnam[1000] ,path[1000];
   int uid;
   char* pname=NULL;
   sprintf(path, "/proc/%d/status", pid);
   FILE *sta = fopen(path, "r");

    if(!sta)
       return 1;

    while(fgets(fuid,1000,sta))
      {
        if(strncmp(fuid, "Uid:",4) == 0)
         {
          sscanf(fuid,"%*s %d",&uid);//reads formatted inp>
         // printf("%d\n",usrp->uid);
          break;
        }
      }
  fclose(sta);

  DIR* et = opendir("/etc");
  if(et == NULL)
  {
    perror("opendir");
    return 1;
  }

 FILE *etnam = fopen("/etc/passwd", "r");
 if(!etnam)
 {
   perror("fopen");
   return 1;
 }
   char *p;
   while(fgets(fnam,1000,etnam))
   {
     p=fnam;
     while (*p) // While there are more characters to process...
     {
       if ( isdigit(*p) || ( (*p=='-'||*p=='+') && isdigit(*(p+1)) ))
      {
        // Found a number
        long val = strtol(p, &p, 10); // Read number
        if(val==uid)
        {
        strtok(fnam,":x");
        pname=fnam;
   //     printf("ccc%s\n",fnam);
        break;
        }
      }
      else // Otherwise, move on to the next character.
      {
        p++;
      }
    if(pname!=NULL) break;
    }
  }
  fclose(etnam);
  closedir(et);
  printf("%s\n",pname);
 
// return fnam;*/
 }




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
 
  


  long unsigned int process_time= proc->utime + proc->stime; //total time spent for the process
 //process_time=process_time+proc->cutime+cstime //with children
  proc->pTime = get_uptime() -(proc->starttime/sysconf (_SC_CLK_TCK));
 // proc->cpu_total_time =100*(((float) process_time /sysconf (_SC_CLK_TCK))/proc->pTime);

  proc->rss=(proc->rss * getpagesize())/1000;//in bytes to kb

  proc->mem = ((float) proc->rss/total_memkb)*100;

  find_usrname(pid);

  printf("%8d",pid);
  printf("%12s",proc->name);
  printf("%5c",proc->statue);
  printf("%11lu",proc->pTime);
  printf("%11.1f", proc->cpu_total_time);
  printf("%12.1f",proc->mem);
 // printf("%12s\n",pname);

  fclose(f);
  free(proc);

 }

void main(int argc, char **argv) {
    int pid;
    sscanf(argv[1], "%d", &pid);
   // printf("pid = %d\t", pid);
  total_memkb= get_total_mem();

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
