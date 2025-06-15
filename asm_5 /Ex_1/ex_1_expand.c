#include <signal.h>   /* for sigaction(), sigemptyset() */
#include <stdio.h>    /* for printf() */
#include <unistd.h>   /* for sleep(), getpid() */


void handle_sigaction(int sig, siginfo_t *info, void *context) {
    printf("Receiving signal %d\n", sig);
    printf("Signal sent by PID: %d\n", info->si_pid);
    printf("Signal code: %d\n", info->si_code);
    printf("User ID of sender: %d\n", info->si_uid);
}

int main(){
    printf("Process ID: %d\n", getpid());
    struct sigaction act;
    act.sa_flags = SA_SIGINFO; /* Use SA_SIGINFO to get more information about the signal */
    act.sa_sigaction = handle_sigaction; /* Use sa_sigaction for additional info */
    sigemptyset(&act.sa_mask);

    sigaction(SIGINT, &act, NULL); /* Register the signal handler for SIGINT */

    while(1){
        sleep(3);
    }
    
    return 0;
}