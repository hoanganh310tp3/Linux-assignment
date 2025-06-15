#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

#define BUFFER_SIZE 5
#define NUM_ITEMS 10

pid_t gettid() {
    return syscall(SYS_gettid);
}

int buffer[BUFFER_SIZE];
int buffer_index = 0;

int producer_sum = 0;
int consumer_sum = 0;
int last_value = 0;

pthread_mutex_t mutex;
pthread_cond_t can_produce;
pthread_cond_t can_consume;

int data_ready = 0;

void* producer(void* arg) {
    pid_t pid = getpid();
    pid_t tid = gettid();
    
    printf("Producer started: PID=%d, TID=%d\n", pid, tid);
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex);
        
        printf("\n Producer is holding mutex !\n");
        
        while (buffer_index >= BUFFER_SIZE) {
            printf("Buffer is full. Producer (TID=%d) is waiting...\n", tid);
            pthread_cond_wait(&can_produce, &mutex);
            printf("Producer (TID=%d) woke up!\n", tid);
        }
        
        int value = i + 1;
        buffer[buffer_index] = value;
        buffer_index++;
        
        producer_sum += value;
        last_value = value;
        
        printf("Producer (TID=%d) produces new value successfully!: %d\n", tid, value);
        printf("Current producer sum: %d\n", producer_sum);

        data_ready = 1;
        pthread_cond_signal(&can_consume);
        
        printf("Producer is releasing mutex!\n\n");
        pthread_mutex_unlock(&mutex);
        
        sleep(5);
    }
    
    printf("Producer (TID=%d) finished. Total produced sum: %d\n", tid, producer_sum);
    return NULL;
}

void* consumer(void* arg) {
    pid_t pid = getpid();
    pid_t tid = gettid();
    
    printf("Consumer started: PID=%d, TID=%d\n", pid, tid);
    
    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex);
        
        printf("\nConsumer is holding mutex\n");
        
        while (buffer_index <= 0 || !data_ready) {
            printf("No data available. Consumer (TID=%d) is waiting...\n", tid);
            pthread_cond_wait(&can_consume, &mutex);
            printf("Consumer (TID=%d) woke up!\n", tid);
        }
        
        buffer_index--;
        int value = buffer[buffer_index];
        
        consumer_sum += value;
        last_value = value;
        
        printf("Consumer (TID=%d) consumes a value successfully!: %d\n", tid, value);
        printf("Current consumer sum: %d\n", consumer_sum);
        
        pthread_cond_signal(&can_produce);
        
        printf("Consumer is releasing mutex\n\n");
        pthread_mutex_unlock(&mutex);
        
        sleep(10);
    }
    
    printf("Consumer (TID=%d) finished. Total consumed sum: %d\n", tid, consumer_sum);
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    pid_t main_pid = getpid();
    pid_t main_tid = gettid();
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&can_produce, NULL);
    pthread_cond_init(&can_consume, NULL);
    
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&can_produce);
    pthread_cond_destroy(&can_consume);
    
    return 0;
}
