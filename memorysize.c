#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>


void main(int argc, char **argv) {

DIR* proc = opendir("/proc");
struct dirent* ent;
long tgid;

if(proc == NULL) {
    perror("opendir(/proc)");
    return 1;
}

     char  line[100], dd[1000];
     long unsigned int p;
     FILE* statusf;
     const char s[2] = ":";
     statusf = fopen("/proc/meminfo", "r");
     if(!statusf)
       return 1;

     while(fgets(line,100, statusf))
      {
        if(strncmp(line, "MemTotal:", 9) == 0)
         { printf("%s\n",line);
          sscanf(line,"%*s %ld",&p);
            printf("%lu\n",p);

       }  }
/*         { printf("%s\n",line);   
        break;}
   }
     char* token = strtok(line, s);
     while( token != NULL ) {
      printf( " %s\n", token );
      token = strtok(NULL, s);
      }

       printf("%s\n",token);
      // fscanf(statusf,"%s %ld",dd, &p);
       // printf("%ld\n",p);

      /*if(isdigit(line)==0){
      printf("%s\n", line);
      fscanf(statusf," %lu" ,&p);
      printf("%lu\n",p);
      break;
       }
      // if(isdigit(line)==0){
       //printf("%ld",line);
       // break;  }
       /* if(!isdigit(line))
       // continue;
        // Ignore "State:" and whitespace
        p = line ; // put  value of line ins>
       // while(isspace(*p)) ++p; //isspace retue>
        printf("%ld", p);
        break;*/

      /*int i = 0;
      while (line[i] != '\0') {
        if (isdigit(line[i++]) == 0) {
            return -1;
        }
     }
     */
    // printf("%ld", line);

 // }


    fclose(statusf);



closedir(proc);

}

