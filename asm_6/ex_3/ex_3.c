#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

void signal_handler(int sig) {
    printf("\nProcess %d interrupted\n", getpid());
    exit(0);
}

int main() {
    signal(SIGINT, signal_handler);
    
    printf("Process PID: %d starting...\n", getpid());
    
    for (int i = 1; i <= 30; i++) {
        printf("Working... %d/30\n", i);
        sleep(2);
    }
    
    printf("Process completed.\n");
    return 0;
}


