/* Additional assignment about function pointers using with signals.*/
/*
The general purpose of the program:
   +) Creating and managing multiple child processes (i use fork()). The child process will inherit all signal handlers from the parent process.
   +) Manipulating and handling signals comprising SIGINT, SIGUSR1, SIGUSR2, and SIGCHLD.
   +) Declaring the use of flag SA_NOCLDSTOP in sigaction() to handle signals like the parent process will not receive SIGCHLD when a child process is stopped or continued.
   +) If i use flag 0 in sigaction(), the parent process will receive SIGCHLD when a child process is stopped or continued. If i use SA_NOCLDSTOP.
   +) Using function pointers for managing signal handlers dynamically. I do not use switch-case or if-else statements to handle signals, but use a dispatch table (array of function pointers) which is functionCallHandler to call the appropriate handler based on the signal received.
   +) Using waitpid() with WNOHANG, WUNTRACED, and WCONTINUED flags to handle child process state changes. The parent process will receive SIGCHLD when a child process changes state (e.g., exits, stops, continues).
   +) Using raise() to send signals to the child process itself.
   +) Using pause() to wait for signals in the parent process.
   +) Using sleep() to simulate the child process running for a certain time before sending signals.
   +) Using exit() to terminate the child process with a specific exit status.
   +) Using getpid() to get the process ID of the current process.
*/

#define _GNU_SOURCE //support for GNU extensions. If i do not use this, WCONTINUED can not be defined and used in the code. WNOHANG and WUNTRACED are defined well without this, because they are old POSIX macros.
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define CHILD_COUNT 3
#define NUM_SIGNALS 3

typedef void (*signalHandler)(int); // The function pointer for creating a dispatch table for signal handlers

void sigint_handler(int sig) {
    printf("Received SIGINT in process %d\n", getpid());
}

void sigusr1_handler(int sig) {
    printf("***Received SIGUSR1 in process %d***\n", getpid());
}

void handle_sigchld(int sig){
    int status;
    pid_t pid;
   // This function is called when a child process changes state (e.g., exits, stops, continues). However if i use SA_NOCLDSTOP, it will not be called when a child process stops.
   // WHOHANG, WUNTRACED and WCONTINUED are used for WIFEXISTED, WIFSTOPPED and WIFCONTINUED macros to check the state of the child process respectively.
    while ((pid = waitpid (-1, &status, WNOHANG | WUNTRACED |  WCONTINUED)) > 0) {
        if (WIFEXITED(status)) {
            printf("| Child process %d exited with status %d\n", pid, WEXITSTATUS(status)); 
        } else if (WIFSTOPPED(status)) {
            printf("| Child process %d stopped by signal %d\n", pid, WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("| Child process %d continued\n", pid);
        } else {
            printf("| Child process %d did not exit normally\n", pid);
        }
    }
}

// signal_list (containing signal constants) and handler_list (holding handler functions for each signal). They are parrallel, both index i in them are mapped respectively.
// When a signal is received, the functionCallHandler will call the corresponding handler function based on the signal number. 
int signal_list[NUM_SIGNALS] = {SIGINT, SIGUSR1, SIGCHLD};

signalHandler handler_list[NUM_SIGNALS] = {
    sigint_handler,
    sigusr1_handler,
    handle_sigchld
};

void functionCallHandler(int sig){
    for(int i=0; i < NUM_SIGNALS; ++i){
        if (sig == signal_list[i]) {
            handler_list[i](sig);
            return;
        }
    }
    printf("Not existing handler for signal %d\n", sig);
}

int main(){
    printf("Parent Process ID: %d\n", getpid());

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // Initialize flags to 0. If you want parent process to receive SIGCHLD when child process is stopped or continued.
    // sa.sa_flags = SA_NOCLDSTOP;// If you want to avoid receiving SIGCHLD when child process is stopped, use SA_NOCLDSTOP. 
    sa.sa_handler = functionCallHandler; // Set the handler function to handle signals dynamically.

    for (int i=0; i < NUM_SIGNALS; ++i) {
        if (sigaction(signal_list[i], &sa, NULL) == -1) {
            perror("sigaction");
            exit(EXIT_FAILURE);
        }
    }

    pid_t child_pids[CHILD_COUNT];

    for(int i=0; i < CHILD_COUNT; ++i) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            printf("==> Child process %d (number: %d) sleep in %d seconds. \n", getpid(), i + 1, 3 + i);
            sleep(3 + i);

            if (i == 0) {
                printf("Child %d: sending SIGSTOP to self.\n", getpid());
                raise(SIGSTOP); // Child process stop itself. It can use this signal hanlder because it is inherited from parent process.

                // If the child process receive continue signal from parent process, it will continue to run.
                printf("Child %d: resumed after STOP.\n", getpid());
            }

            printf("Child process %d sending SIGUSR1 to itself by raise.\n", getpid());
            raise(SIGUSR1); // Child sends SIGUSR1 to itself

            exit(10 + i); // Exit with a unique status for each child, be easy to identify with others
        } else {
            child_pids[i] = pid; // Parent process stores child PID for later sending continue signal.
        }
    }


    sleep(10);
    printf("Parent process sends SIGCONT to child process %d. \n", child_pids[0]);
    kill(child_pids[0], SIGCONT); // Continue the first child process
    
    // Parent process waits in eternity for signals 
    while (1) {
        pause(); // Wait for signals. Can ctrl + C to test SIGINT handling
    }

    return 0;
}






