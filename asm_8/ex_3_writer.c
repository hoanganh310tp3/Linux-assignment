#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    key_t key = ftok("file_null", 65);
    key_t sem_key = ftok("file_null", 66);
    
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    printf("Writer: shmid = %d\n", shmid);
    
    int semid = semget(sem_key, 1, 0666|IPC_CREAT);
    if (semid == -1) {
        perror("semget failed");
        exit(1);
    }
    
    union semun arg;
    arg.val = 0;
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        perror("semctl failed");
        exit(1);
    }
    
    char* str = (char*) shmat(shmid, NULL, 0);
    if (str == (char*)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    printf("Writer: Enter message to write: ");
    fgets(str, 1024, stdin);
    str[strcspn(str, "\n")] = 0;
    
    printf("Writer: Data written to shared memory: %s\n", str);
    
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;    
    sb.sem_flg = 0;
    
    if (semop(semid, &sb, 1) == -1) {
        perror("semop signal failed");
        exit(1);
    }
    
    printf("Writer: Signaled reader that data is ready\n");
    
    shmdt(str);
    return 0;
}
