#include <stdio.h> // used for printf(), perror(), fflush()
#include <stdlib.h> // used for exit()
#include <unistd.h> // used for read(), write(), close(), pipe(), socketpair()
#include <sys/epoll.h> // used for epoll_create1(), epoll_ctl(), epoll_wait()
#include <sys/socket.h> // used for socketpair()
#include <fcntl.h> // used for fcntl()


#define MAX_EVENTS 2
#define BUF_SIZE 1024

void make_nonblocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0); /* Get the current file descriptor flags */
    fcntl(fd, F_SETFL, flags | O_NONBLOCK); /* Turn on non-blocking mode for the file */
}

int main() {
    int epfd = epoll_create1(0); //file descriptor returned by epoll_create which identifies the epoll instance in the kernel.
    if (epfd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1) {
        perror("socketpair");
        exit(EXIT_FAILURE);
    }

    make_nonblocking(0); // stdin (fd 0)
    make_nonblocking(sv[1]); // read socket

    struct epoll_event ev, events[MAX_EVENTS];

    ev.events = EPOLLIN;
    ev.data.fd = 0;
    epoll_ctl(epfd, EPOLL_CTL_ADD, 0, &ev); //block stdin (fd 0)

    ev.events = EPOLLIN;
    ev.data.fd = sv[1];
    epoll_ctl(epfd, EPOLL_CTL_ADD, sv[1], &ev);//block socketpair (fd sv[1])

    printf("Monitoring stdin and socketpair. Type something or wait for event...\n");

    while (1) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
        perror("epoll_wait");
        break;
    }

    for (int i = 0; i < nfds; ++i) {
        int fd = events[i].data.fd;
        char buf[BUF_SIZE];
        int len = read(fd, buf, sizeof(buf) - 1);

        if (len > 0) {
            buf[len] = '\0';
            if (fd == 0) {
                printf("User typed: %s\n", buf);
                write(sv[0], buf, len);
            } else if (fd == sv[1]) {
                printf("Message from Socketpair: %s\n", buf);
                fflush(stdout);
            }
        }
    }
}
close(sv[0]);
close(sv[1]);
close(epfd);
return 0;
}
