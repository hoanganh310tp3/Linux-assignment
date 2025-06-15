#include <string.h>
#include <sys/msg.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(){
    int msqid = 0;

    struct message {
        long type;
        char text[100];
    } msg;

    msg.type = 1;
    strcpy(msg.text, "This is a first message");

    msgsnd(msqid, (void*)&msg, strlen(msg.text)+1, IPC_NOWAIT);

    strcpy(msg.text, "This is a second message");

    msgsnd(msqid, (void*)&msg, strlen(msg.text)+1, IPC_NOWAIT);

    return 0;

}

/* For creating the message queue, we use command "ipcmk -Q" */