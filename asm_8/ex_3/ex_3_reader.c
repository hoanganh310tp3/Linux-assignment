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
    
    int shmid = shmget(key, 1024, 0666);
    if (shmid == -1) {
        perror("shmget failed - run writer first");
        exit(1);
    }

    printf("Writer: shmid = %d\n", shmid);

    int semid = semget(sem_key, 1, 0666);
    if (semid == -1) {
        perror("semget failed - run writer first");
        exit(1);
    }
    
    printf("Reader: Waiting for writer to finish writing...\n");
    
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;   
    sb.sem_flg = 0;
    
    if (semop(semid, &sb, 1) == -1) {
        perror("semop wait failed");
        exit(1);
    }
    
    printf("Reader: Writer has finished, now reading data...\n");
    
    char* str = (char*) shmat(shmid, NULL, 0);
    if (str == (char*)-1) {
        perror("shmat failed");
        exit(1);
    }
    
    printf("Reader: Data read from shared memory: %s\n", str);
    
    shmdt(str);
    shmctl(shmid, IPC_RMID, NULL);
    
    union semun arg;
    semctl(semid, 0, IPC_RMID, arg);
    
    printf("Reader: Cleanup completed\n");
    return 0;
} 
