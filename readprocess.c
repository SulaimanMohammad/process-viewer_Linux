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

#define NORMAL_COLOR  "\x1B[0m"
#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

#define update() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", x, y)


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
   // long unsigned int pTime;
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


//dependent function 
 void get_total_mem(struct cpu_info *cpud)
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
          sscanf(line,"%*s %ld",&cpud->total_memkb );//reads formatted input from a string.
         // printf("%ld\n", total_memkb);
        break;
        }
      }
 fclose(statusf);
 }


 void get_sharedmemory_size(struct state  *proc, int pid)
 {
   char filename[1000];
   sprintf(filename, "/proc/%d/statm", pid);
   FILE* memf = fopen(filename, "r");
   if(!memf)
    { perror("fopen");  return 1;}
    fscanf(memf,"%*lu %*lu %lu",&proc->shared_m_size);
    fclose(memf);
    proc->shared_m_size=( proc->shared_m_size) * getpagesize();
 }






//dependent 
 char* find_usrname(int pid)
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
        break;
        }
      }
      else // Otherwise, move on to the next character.
      {
        p++;
      }

    }
   if(pname!=NULL) break;

  }
  fclose(etnam);
  closedir(et);

 return pname;
 }



//dependent 
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


//dependent 
 void read_statFile (struct state  *proc, int pid)
 {
   char filename[1000];
    sprintf(filename, "/proc/%d/stat", pid);
    FILE *f = fopen(filename, "r");
   // long long process_start_time_since_boot;

    fscanf(f, "%*d %s %c %d %*d %*d %*d %*d %*lu %*lu %*lu %*lu %*lu %lu %lu "
              "%ld %ld %ld %ld %*ld %*ld %llu %lu %ld",
                proc->name,&proc->statue,&proc->ppid,
                &proc->utime,&proc->stime,&proc->cutime,
                &proc->cstime,&proc->priority, &proc->nice,
                &proc->starttime, &proc->vsize, &proc->rss);
    fclose(f);

 }


//dependent 
 void get_total_cpu_usage(struct cpu_info *cpud)
 {
   char cpu_data[1000];
   FILE* cpuf = fopen("/proc/stat", "r");
   if(!cpuf)
    { perror("fopen");  return 1;}

  fscanf(cpuf,"%*s %llu %llu %llu %llu %llu %llu %llu %llu",
       &cpud->user,&cpud->nice,&cpud->system,&cpud->idle, &cpud->iowait, &cpud->irq, &cpud->softirq, &cpud->steal);
  cpud->total_usage= cpud->user+cpud->nice+cpud->system+cpud->idle+cpud->iowait+cpud->irq+cpud->softirq+cpud->steal;
  fclose(cpuf);
 }


//needs total_cpu_usage , read_stat
 void get_process_cpu_usage(struct cpu_info *cpud , struct state  *proc, int pid)
 {
  read_statFile(proc,pid);//the data here will be used for the first measurment>
  long long unsigned int  process_time1 = proc->stime + proc->utime; //total time spent for the process
  get_total_cpu_usage(cpud);
  long long unsigned int t_cpu_u1= cpud->total_usage;
 // it works but test it with instance work 
  struct timespec wait_time={0};
  wait_time.tv_nsec=140000000;
  nanosleep(&wait_time, NULL);

 // sleep(1);
  //second cpu measur
  get_total_cpu_usage(cpud);
  long long unsigned int t_cpu_u2= cpud->total_usage;
  read_statFile(proc,pid);
  long long unsigned int  process_time2 = proc->stime + proc->utime; //total time spent for the process
  if ((t_cpu_u2 - t_cpu_u1)!=0) proc->cpu_total_time=((process_time2-process_time1)/(float)(t_cpu_u2 - t_cpu_u1))* 100.0 *sysconf(_SC_NPROCESSORS_ONLN);
 // printf("%f\n", proc->cpu_total_time);

 }





 /*.................................................................*/
 /*.................................................................*/
 // find the time of process it is thundred of seconds 
 bool process_isPerent(int pid)// if it is parent return true 
 {
  bool isPerent=false;
  char filename[1000];
  sprintf(filename, "/proc/%d/task", pid);
  DIR * ptype = opendir(filename); // open the  dir
  if(ptype==NULL) return; // if was not able return
  struct dirent * dir; // for the directory entries
  while((dir = readdir(ptype))!= NULL) // if we were able to read somehting from the directory
    {
      if(dir-> d_type != DT_DIR) // if the type is not directory just print it with blue
      {
       break;
      }
     /* (.) is a directory entry for current directory (..) is a directory entry for the directory one level up in hierarchy*/
      else
      if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 ) // if it is a directory
      {
          /*
         the first dir in the task is the dir of the process parent  
         if the process is parent then the first dir has the same number of the process ID 
         else the process Id and the firt dir number are different 
          */
          if(atoi(dir->d_name)==pid) 
             {
             isPerent=true;
             }
           break;
      }
    }
  closedir(ptype); // finally close the directory
  return isPerent;
 }

/*..............................................*/
/*..............................................*/

 void comput_process_time(struct state  *proc, int pid)
 {
 //check if the process is parent or child 
  if(process_isPerent(pid))
  {
  proc->process_time =  proc->utime + proc->stime + proc->cutime + proc->cstime + proc->nice;
  }
  else //it is child  here we read the file schedstat
  {
    char filename[1000];
    sprintf(filename, "/proc/%d/schedstat", pid);
    FILE *sch = fopen(filename, "r");
    fscanf(sch,"%lu",&proc->process_time);//it should be divided by 1000000000
    proc->process_time=(int)floor(((proc->process_time)/1000000000));
    fclose( sch);
  }
  /* int hour=(int)floor(proc->pTime)/3600;
  int min =(int)floor((proc->pTime) - (3600* hour))/60;
  int second =(int)floor((proc->pTime) - (3600* hour)) -(min*60);*/

 }


