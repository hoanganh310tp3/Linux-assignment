#define _GNU_SOURCE 
#include <signal.h> /* For sigemptyset(), sigaddset(), sigprocmask(), sigpending(), sigismemeber(), SIGINT, NSIG */
#include <stdio.h> /* For printf(), perror() */
#include <unistd.h>/* For sleep() */
#include <stdlib.h>/* For exit() */

int main (){
    sigset_t block_set, pending_set;

    sigemptyset(&block_set);
    sigaddset(&block_set, SIGINT);
    sigprocmask(SIG_BLOCK, &block_set, NULL); // Block SIGINT
     
    printf("You can send SIGINT(ctrl + c).\n");
    sleep(5);

    if(sigpending(&pending_set) == -1){
        perror("sigpending");
        return 1;
    }

    int pending_count = 0;

    for(int i=1; i < NSIG; ++i){
        if(sigismember(&pending_set, i)){
            printf(" Signal SIGINT is pending. \n");
            pending_count =1;
            exit(1);
        } 
    }
    
    if(!pending_count){
        printf("No signal is pending.\n");
    }

    while(1){
        printf("Waiting for SIGINT...\n");
        sleep(1);
    }

    return 0;
}