#include "readprocess.h"
#define update() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", x, y)


void process_value(int pid)
 {
  struct cpu_info* cpud;
  cpud = malloc(sizeof *cpud);

  struct state* proc;
  proc = malloc(sizeof *proc);

  read_statFile(proc,pid);

  get_process_cpu_usage(cpud,proc, pid);

  comput_process_time(proc, pid);

  proc->rss=(proc->rss * getpagesize())/1000;//in bytes to kb

  get_total_mem(cpud);

  proc->mem = ((float) proc->rss/cpud->total_memkb)*100;

  find_usrname(pid);

  get_sharedmemory_size(proc,pid);

  proc->vsize=((int)(proc->vsize)/(int)1000000);//vsize in byte

  printf("%8d",pid);
  printf("%12s",find_usrname(pid));
  printf("%7ld",proc->priority);
  printf("%8ld",proc->nice);
  printf("%10dM",proc->vsize);
  printf("%9ldM",(proc->rss)/1000);
  printf("%8luM",(proc->shared_m_size)/1000000);
  printf("%8c",proc->statue);
  printf("%11.2f",proc->cpu_total_time );
  printf("%9.2f",proc->mem);
  printf("%9lu",proc->process_time);
  printf("%12s\n",proc->name);
 // fflush(stdout);

 free(cpud);
 free(proc);
 }

bool kernal_thread(int pid)//kenale thread has cmdline  empty 
{
 char filename[1000];
   sprintf(filename, "/proc/%d/cmdline", pid);
   FILE* kth = fopen(filename, "r");
   if(!kth)
    { perror("fopen");  return 1;}

    int c = fgetc(kth);
    if (c == EOF)
    {
    return true;
    }
    else
    return false;

    fclose(kth);
}


void main(int argc, char **argv) {
//int pids[4194303]={0};//there is no pid is zero
int pids;

#pragma omp parallel
{
 while(1){
  DIR* proc = opendir("/proc");
  struct dirent* ent;

  if(proc == NULL) {
    perror("opendir(/proc)");
    return 1;
  }

 while(ent = readdir(proc))
 {
  if(!isdigit(*ent->d_name))
     continue;
  pids = strtol(ent->d_name, NULL, 10);
  if(!kernal_thread(pids))
  //call the function
  // update();
 /* printf("======================================================================================================================\n");
  printf("|   PID   |  USER   |   PRI   |   NI   |  VIRT  |   RES   |   SHR   |   S   |   CPU%   |   MEM%   |  TIME+ | COMMAND  \n");
  printf("======================================================================================================================\n");
 */
 process_value(pids);

 }
    update();
    //sleep(1);
    gotoxy(0, 0);
    usleep(100); 
 closedir(proc);

}
    //update();
    //sleep(1);
   // gotoxy(0, 0);
   // usleep(100); 
}
}
