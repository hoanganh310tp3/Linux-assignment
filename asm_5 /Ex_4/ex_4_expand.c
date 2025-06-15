/* In this code, process B (child process) will block signal SIGUSR1 sent by process A (parent process). After process B remove blocking SIGUSR1, process A can send this signal to process B*/

#define _GNU_SOURCE 
#include <stdio.h>      /* For printf() */
#include <stdlib.h>     /* For exit() */
#include <signal.h>     /* For signal handling functions (signal, sigprocmask, sigemptyset, sigaddset) */
#include <unistd.h>     /* For fork(), getpid(), getppid(), sleep() */
#include <sys/wait.h>   /* For wait() */

void sigusr1_handler(int sig) {
    printf("Process B (PID: %d) received signal from process A: %d\n", getpid(), sig);
}

int main(){
    pid_t pid;

    pid = fork();

    if(pid < 0){
        perror ("fork");
        exit(1);
    }

    else if (pid == 0){
        sleep(1);
        sigset_t set;
        signal(SIGUSR1, sigusr1_handler);
        sigemptyset(&set);
        sigaddset(&set, SIGUSR1);
        printf("Process B (PID: %d) is blocking SIGUSR1 from process A (PID: %d)\n", getpid(), getppid());

        sigprocmask(SIG_BLOCK, &set, NULL);

        sleep(3);

        printf("Process B (PID: %d) is unblocking SIGUSR1 from Process A (PID: %d)\n", getpid(), getppid());
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        exit(0);
    }

    else {
        sleep(2); // Ensure Process B has set up its signal handler and blocked SIGUSR1
        printf("Process A (PID: %d) is sending SIGUSR1 to Process B (PID: %d)\n", getpid(), pid);
        kill(pid, SIGUSR1); // Send SIGUSR1 to child process B
        sleep(5); // Allow time for Process B to handle the signal
        wait(NULL); // Process A waits for child process B to finish, avoiding zombie process
    }
    return 0;
}