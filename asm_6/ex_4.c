#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void ChildrenProcess(int parent_index, int child_index){
	printf("Child process [PID:%d] of Parent process [PPID: %d] is created!\n", getpid(), getppid());

	sleep(5);

	printf("Child process [PID:%d] of Parent process [PPID: %d] is terminated!\n", getpid(), getppid());
 
	exit(0);
}

void ParentProcess(int parent_index, int y){
       pid_t children[y];

       for(int i=0; i<y; ++i){
	       pid_t pid = fork();

	       if(pid == 0){
		       ChildrenProcess(parent_index,i);}
	       else if(pid >0){
		       children[i] = pid;
	       } else{
		       printf("The fork for creating children process is failed!\n");
	       }
       }

      for(int i=0; i<y; ++i){
	      waitpid(children[i], NULL, 0);
      }
    
      printf("Parent process [PID:%d] wait all children sucessfully!\n", getpid());

      exit(0);

}

int main(){

	int x, y;
    printf("Type the number of father process: ");
	scanf("%d", &x);
	printf("Type the number of children process: ");
	scanf("%d", &y);
	printf("----------------------------------------------\n");
	
	pid_t parent[x];

	for(int i=0; i < x; ++i){
		pid_t pid = fork();

		if(pid == 0){
			ParentProcess(i,y);}
		else if( pid >0){
			parent[i] = pid;
		} else{
			printf("The fork for creating father process is failed.\n");
		}
		}
	
	for(int i=0; i < x; ++i){
		waitpid(parent[i], NULL, 0);
	}

	printf("All parent and their children are terminated!\n");

	return 0;

}


























