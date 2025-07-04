#include <stdio.h> // used for printf()
#include <stdlib.h> // used for exit()
#include <signal.h> // used for SIGUSR1, SIGUSR2, siganl()
#include <unistd.h> // used for getpid(), pause()

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1: Hello from SIGUSR1 handler!\n");
    } else if (signum == SIGUSR2) {
        printf("Received SIGUSR2: Exiting program.\n");
        exit(0);  
    }
}

int main() {
    
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        perror("Error registering SIGUSR1 handler");
        return 1;
    }

    if (signal(SIGUSR2, signal_handler) == SIG_ERR) {
        perror("Error registering SIGUSR2 handler");
        return 1;
    }

    printf("Process PID: %d\n", getpid());
    printf("Waiting for SIGUSR1 or SIGUSR2...\n");

    
    while (1) {
        pause(); 
    }

    return 0;
}
