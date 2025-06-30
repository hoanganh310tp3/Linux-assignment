#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void ParentProcess(){
        printf("Process id of the parent section: %d.\n", getpid());
}

void ChildProcess(){
         printf("Process id of the children section: %d.\n", getpid());
}

int main(){
   pid_t  id = fork();

   if (id > 0){
           ParentProcess();
	   sleep(50);
	   wait(NULL);
   } else if (id == 0){
           ChildProcess();
	   exit(0); // exit the child process. Child becomes a zombie process.
   } else{
           printf("The fork is failed");
           return 1;
   }
   return 0;
}


