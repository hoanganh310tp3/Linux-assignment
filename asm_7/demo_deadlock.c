#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock1;
pthread_mutex_t lock2;

int counter = 0;

void* increment_1(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock1);
        sleep(5);
        pthread_mutex_lock(&lock2);
        printf("thread1");
        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);
        }
    return NULL;
}

void* increment_2(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock2);
        sleep(5);
        pthread_mutex_lock(&lock1);
        printf("thread 2");
        pthread_mutex_unlock(&lock1);
        pthread_mutex_unlock(&lock2);
    return NULL;
}
}


int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock1, NULL); 
    pthread_mutex_init(&lock2, NULL); 

    pthread_create(&t1, NULL, increment_1, NULL);
    pthread_create(&t2, NULL, increment_2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&lock2); 
    pthread_mutex_destroy(&lock1); 

    return 0;
}

