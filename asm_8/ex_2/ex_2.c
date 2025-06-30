#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem;

typedef struct {
    int thread_id;
    int sleep_time;
} thread_data_t;

void* thread_func(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    sem_wait(&sem);
    printf("Thread %d entered critical section.\n", data->thread_id);
    sleep(data->sleep_time);
    printf("Thread %d left critical section.\n", data->thread_id);
    sem_post(&sem);
    
    return NULL;
}

int main() {
    pthread_t threads[6];
    thread_data_t thread_data[6] = {
        {1, 2}, {2, 3}, {3, 1}, {4, 3}, {5, 2}, {6, 1}
    };
    
    sem_init(&sem, 0, 3);


    for (int i = 0; i < 6; i++) {
        pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
    }

    for (int i = 0; i < 6; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem);
    
    return 0;
}
