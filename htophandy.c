#include "readprocess.h"
//macros to clear the output and refrech the output
#define update() printf("\033[H\033[J")
#define gotoxy(x, y) printf("\033[%d;%dH", x, y)

//the main function to calculated the data of the process
void process_value(int pid)
{
  //object for the cpu pid data
  struct cpu_info* cpud;
  cpud = malloc(sizeof *cpud);

  //object for the process data
  struct state* proc;
  proc = malloc(sizeof *proc);

   //object for the memory data
  struct memorydata* memo;
  memo = malloc(sizeof *memo);

  read_statFile(proc, pid);// read the process data

  get_process_cpu_usage(cpud, proc, pid);//calculated the process cpu usage

  comput_process_time(proc, pid);//since when the process is runing

  get_memory_usage(proc,memo);

  // find_usrname(pid);

  get_sharedmemory_size(memo,pid); // shared memory for the process


  printf("%8d",pid);
  printf("%12.10s",find_usrname(pid));
  printf("%7ld",proc->priority);
  printf("%8ld",proc->nice);

  if(proc->vsize > 10000)
  {
   proc->vsize=(int)((proc->vsize)/1000000);//vsize in byte
   printf("%10dM",proc->vsize);
  }
  else
  printf("%10d",proc->vsize);

  if(proc->rss > 1000)
  {
    proc->rss=(proc->rss)/1000;
    printf("%9ldM",proc->rss);

  }
  else
  printf("%9ld",proc->rss);

  printf("%8luM",(memo->shared_m_size)/1000000);
  printf("%8c",proc->statue);
  printf("%11.2f",cpud->cpu_total_time );
  printf("%9.2f",memo->mem);
  printf("%9lu",proc->process_time);
  printf("%12.10s\n",proc->name);
  fflush(stdout);


  //free the object and releae the memory
  free(cpud);
  free(proc);
  free(memo);
 }

// determin if the process is a theread of the kernel
// when the process has cmdline empty then it is kernel's thread
bool kernal_thread(int pid)
{
  char filename[1000];
  sprintf(filename, "/proc/%d/cmdline", pid);
  FILE* kth = fopen(filename, "r");

  if(!kth)
  {
   perror("fopen");
  }
  //get when char if it is the end of the file then it is empty
  int c = fgetc(kth);
  if (c == EOF)
  {
   return true;
  }
  else
  return false;

  fclose(kth);
}


void main(int argc, char *argv[])
{

int i=0;
  int pids;
  printf("======================================================================================================================\n");
  printf("|   PID   |  USER   |   PRI   |   NI   |  VIRT  |   RES   |   SHR   |   S   |   CPU%   |   MEM%   |  TIME+ | COMMAND  \n");
  printf("======================================================================================================================\n");

 //#pragma omp parallel
 //{

  if(argc>=2)
  {
    DIR* procd = opendir("/proc");
    struct dirent* ent;
    if(procd == NULL)
    {
      perror("opendir");
    }
    while(ent = readdir(procd))
    {
      if(!isdigit(*ent->d_name))
      continue;
      pids = strtol(ent->d_name, NULL, 10);

      //check if there is instruction to show specific process
      if(strcmp(argv[1],"-i") && (strtol(argv[2], NULL, 10) == pids))
      {
        while(1)
        {
         process_value(pids);
        }
      }
    }
    closedir(procd);

  }
 //show all the process
 else
 {
  while(1)
  {
   DIR* procd = opendir("/proc");
   struct dirent* ent;

   if(procd == NULL)
   {
     perror("opendir");
   }

   while(ent = readdir(procd))
   {
     if(!isdigit(*ent->d_name))
     continue;
     pids = strtol(ent->d_name, NULL, 10);

     if(!kernal_thread(pids))
     //call the calculation function
     process_value(pids);

    }
    
   closedir(procd);

  }
 }
}
