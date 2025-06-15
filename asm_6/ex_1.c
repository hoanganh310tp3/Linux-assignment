#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void ParentProcess(){
        printf("Process id of the parent section: %d.\n", getpid());
}

void ChildProcess(){
         printf("Process id of the child section: %d.\n", getpid());
         printf("The process id of the child's parent: %d.\n", getppid());
}

int main(){
   pid_t id = fork();

   if (id > 0){
           ParentProcess();
	   wait(NULL);
	   exit(0);
   } else if (id == 0){
           ChildProcess();
	   exit(0);
   } else{
           printf("The fork is failed");
   }
   return 0;
}

