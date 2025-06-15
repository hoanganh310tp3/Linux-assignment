#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM 5

int a_done = 0;
int b_done = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_A_done = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_B_done = PTHREAD_COND_INITIALIZER;

void* Thread_1(void* arg) {
    for(int i = 0; i< NUM; i++){
        pthread_mutex_lock(&mutex);
        while (a_done != 0) {
            pthread_mutex_unlock(&mutex);
            usleep(1000);  
            pthread_mutex_lock(&mutex);
        }

        printf("Thread 1 is processing.\n");
        sleep(1);
        a_done = 1;
        pthread_cond_signal(&cond_A_done);
        pthread_mutex_unlock(&mutex);
    }
    printf("****Thread 1 finished.****\n");
    return NULL;
}  


void* Thread_2(void* arg) {
    
    for(int i=0; i<NUM; i++){
        pthread_mutex_lock(&mutex);
        while (a_done == 0){
            pthread_cond_wait(&cond_A_done, &mutex);
        }
        printf("Thread 2 is processing.\n");
        sleep(1);
        a_done =0;
        b_done =1;
        pthread_cond_signal(&cond_B_done);
        pthread_mutex_unlock(&mutex);
    }
    printf("****Thread 2 finished.****\n");
    return NULL;
}

void* Thread_3(void* arg){
   
    for(int i=0; i<NUM; i++){
        pthread_mutex_lock(&mutex);
        while ( b_done == 0){
            pthread_cond_wait(&cond_B_done, &mutex);
        }
        printf("Thread 3 is processing.\n");
        sleep(1);
        b_done =0;
        pthread_mutex_unlock(&mutex);
    }   
    printf("****Thread 3 finished.****\n");
    return NULL;
} 

int main() {
    pthread_t thread_a, thread_b, thread_c;
    

    pthread_create(&thread_a, NULL, Thread_1, NULL);
    pthread_create(&thread_b, NULL, Thread_2, NULL);
    pthread_create(&thread_c, NULL, Thread_3, NULL);

    
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    pthread_join(thread_c, NULL);
    
    
    return 0;
}
