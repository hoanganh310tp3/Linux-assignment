#define _GNU_SOURCE 
#include <stdio.h> /* For printf(), perror() */
#include <signal.h>/* For sigprocmask(), sigempty(), sigaddset(), sigismember(), SIGINT, SIGTERM, SIGQUIT, NSIG */
#include <unistd.h>/* For sleep() */
#include <string.h>/* For strsignal() */

int signals_to_block[] = {SIGINT, SIGTERM, SIGQUIT};
int n = 3;

void printSignalMask(){
    sigset_t current_mask;
    if (sigprocmask(SIG_BLOCK, NULL, &current_mask) == -1) {
        perror("sigprocmask");
        return;
    }

    printf("All the signals that are currently blocked are shown below.\n");
    int mask_found = 0;
    for (int sig = 1; sig < NSIG; ++sig) {
        if (sigismember(&current_mask, sig)) {
            printf("  %2d - %s\n", sig, strsignal(sig));
            mask_found = 1;
        }
    }

    if(!mask_found) {
            printf("No signals are currently blocked.\n");
        }
}

void blockSignal(){
    printf("All three signals SIGINT, SIGTERM, SIGQUIT are blocked. \n");
    sigset_t block_set;
    sigemptyset(&block_set);
    for(int i = 0; i < n; i++){
        sigaddset(&block_set, signals_to_block[i]);
    }
    sigprocmask(SIG_BLOCK, &block_set, NULL);
}

void unBlockSignal(){
    printf("All three signals SIGINT, SIGTERM, SIGQUIT are unblocked. \n ");
    sigset_t unblock_set;
    sigemptyset(&unblock_set);
    for( int i=0; i < n; i++){
        sigaddset(&unblock_set, signals_to_block[i]);
    }
    sigprocmask(SIG_UNBLOCK, &unblock_set, NULL);
}
    

int main(){
    
    /* Block specific signals */
    blockSignal();

    sleep(3);

    printSignalMask();
 
    sleep(3);

    /* Unblock specific signals */
    unBlockSignal();
    /* For instance, if you press ctrl +c in the time blockSignal() excecuting, the process do not receive this signal. After that, right the time unBlockSignal() executing, the process will receive the previous ctrl + C*/
    
    while(1){
        sleep(1);
    }
    
    return 0;
    
}