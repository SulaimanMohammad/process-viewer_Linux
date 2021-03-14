#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include "client.h"
#include "capteur.h"
#include "server.h"
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

  proc->vsize=((int)(proc->vsize)/(int)1000000);//vsize in byte

  printf("%8d",pid);
  printf("%12s",find_usrname(pid));
  printf("%7ld",proc->priority);
  printf("%8ld",proc->nice);
  printf("%10dM",proc->vsize);
  printf("%9ldM",(proc->rss)/1000);
  printf("%8luM",(memo->shared_m_size)/1000000);
  printf("%8c",proc->statue);
  printf("%11.2f",cpud->cpu_total_time );
  printf("%9.2f",memo->mem);
  printf("%9lu",proc->process_time);
  printf("%12s\n",proc->name);
  //char buf[100];
  //write(1,buf,proc->name);
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


void affiche_hot_client()
{

	int pids;
	  printf("======================================================================================================================\n");
	  printf("|   PID   |  USER   |   PRI   |   NI   |  VIRT  |   RES   |   SHR   |   S   |   CPU%   |   MEM%   |  TIME+ | COMMAND  \n");
	  printf("======================================================================================================================\n");

	#pragma omp parallel
	//{
	  while(1)
	  {
	   DIR* proc = opendir("/proc");
	   struct dirent* ent;

	   if(proc == NULL)
	   {
	     perror("opendir(/proc)");
	   }

	   while(ent = readdir(proc))
	   {
	     if(!isdigit(*ent->d_name))
	     continue;
	     pids = strtol(ent->d_name, NULL, 10);
	     if(!kernal_thread(pids))
	     //call the calculation function
	     process_value(pids);

	    }
	    update();
	    //sleep(1);
	    gotoxy(0, 0);
	    usleep(100);

	   closedir(proc);
	   printf("======================================================================================================================\n");
	   printf("|   PID   |  USER   |   PRI   |   NI   |  VIRT  |   RES   |   SHR   |   S   |   CPU%   |   MEM%   |  TIME+ | COMMAND  \n");
	   printf("======================================================================================================================\n");
	 }
	//}
}
void my_client()
{
	struct addrinfo *addr_info =NULL;
  struct addrinfo hints;

	memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

  int ret = getaddrinfo(NULL, SERVER_PORT, &hints, &addr_info);

  if (ret < 0)
    {
      herror("getaddrinfo");
      exit(EXIT_FAILURE);
    }

    struct addrinfo *tmp = NULL;
    int sock = -1;
    int connected = 0;

    for (tmp = addr_info; tmp != NULL; tmp = tmp->ai_next)
      {
        sock = socket(tmp->ai_family, tmp->ai_socktype, tmp->ai_protocol);

        if (sock < 0)
          {
            perror("socket");
            continue;
          }

        int ret = connect(sock, tmp->ai_addr, tmp->ai_addrlen);

        if (ret < 0)
          {
            perror("connect");
            continue;
          }

        connected = 1;
        break;
      }

		affiche_hot_client();

    shutdown(sock, SHUT_RDWR);

    close(sock);
}
