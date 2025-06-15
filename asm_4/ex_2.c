#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

int main() {
    fd_set readfds;
    struct timeval timeout;
    int retval;

    printf("Please enter input within 10 seconds:\n");

    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds); 

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;


    retval = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
    // the first argument need to be plus one because select() scan all FD from 0 to nfds -1 

    if (retval == -1) {
        perror("select()");
        return 1;
    } else if (retval) {

        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            printf("You entered: %s", buffer);
        } else {
            printf("Error reading input.\n");
        }
    } else {
        
        printf("No input is received within 10 seconds.\n");
    }

    return 0;
}
