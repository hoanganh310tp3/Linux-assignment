#define _GNU_SOURCE
#include <signal.h>  /* for signal handling function (sigaction(), sigemptyset(), sigaddset(), SIGINT */
#include <stdio.h>   /* for printf() */
#include <unistd.h>  /* for getpid(), sleep() */

void handle_sigaction(int sig) {
    printf("Receiving signal %d \n", sig);
}

int main (){
    printf("%d\n",getpid());
    struct sigaction act;
    act.sa_flags =  0;
    act.sa_handler = handle_sigaction;

    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);

    sigaction(SIGINT, &act, NULL);
    while(1){
        sleep(3);
    }
    return 0;
}