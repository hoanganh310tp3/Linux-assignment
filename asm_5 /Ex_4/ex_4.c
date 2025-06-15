/* In this code, process A (parent process) will block signal SIGUSR1 sent by process B (child process). After process A remove blocking SIGUSR1, process B can send this signal to process A*/

#define _GNU_SOURCE 
#include <stdio.h>      /* For printf() */
#include <stdlib.h>     /* For exit() */
#include <signal.h>     /* For signal handling functions (signal, sigprocmask, sigemptyset, sigaddset) */
#include <unistd.h>     /* For fork(), getpid(), getppid(), sleep() */
#include <sys/wait.h>   /* For wait() */

void sigusr1_handler(int sig) {
    printf("Process A (PID: %d) received signal: %d\n", getpid(), sig);
}

int main() {
    pid_t pid;
    sigset_t set;

    signal(SIGUSR1, sigusr1_handler);

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

/* This action blocking signal is from process A (parent process) */
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid = fork();

/* Process B is child process and Process A is parent process */
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    else if (pid == 0) {
        sleep(1);

        printf("Process B (PID: %d) sending SIGUSR1 to Process A (PID: %d)\n", getpid(), getppid());
        kill(getppid(), SIGUSR1); // Send SIGUSR1 to parent process A

        sleep(3);

        printf("Process B sending SIGUSR1 to process A again\n");
        kill(getppid(), SIGUSR1);

        exit(0);
    }

    else {
        printf("Process A (PID: %d) is blocking SIGUSR1...\n", getpid());
        sleep(5);

        printf("Process A is now unblocking SIGUSR1.\n");
        sigprocmask(SIG_UNBLOCK, &set, NULL);

        sleep(3); /* sleep for ensuring having time for receving second signal from process B after unblocking */
        wait(NULL); /* Process A wait for child process B, avoiding zombie process*/
    }

    return 0;
}
