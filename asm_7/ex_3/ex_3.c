#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>

// Buffer size for producer-consumer problem
#define BUFFER_SIZE 5
// Total number of items to produce/consume
#define NUM_ITEMS 10

// Function to get thread ID using syscall
pid_t gettid() {
    return syscall(SYS_gettid);
}

// Shared buffer and index
int buffer[BUFFER_SIZE];
int buffer_index = 0;

// Track sums and last value produced/consumed
int producer_sum = 0;
int consumer_sum = 0;
int last_value = 0;

// Synchronization primitives
pthread_mutex_t mutex;
pthread_cond_t can_produce;
pthread_cond_t can_consume;

// Flag to indicate data is ready for consumption
int data_ready = 0;

// Producer thread function
void* producer(void* arg) {
    pid_t pid = getpid();
    pid_t tid = gettid();
    
    printf("Producer started: PID=%d, TID=%d\n", pid, tid);
    
    // Produce NUM_ITEMS items
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Lock mutex before accessing shared resources
        pthread_mutex_lock(&mutex);
        
        printf("\n Producer is holding mutex !\n");
        
        // Wait while buffer is full
        while (buffer_index >= BUFFER_SIZE) {
            printf("Buffer is full. Producer (TID=%d) is waiting...\n", tid);
            pthread_cond_wait(&can_produce, &mutex);
            printf("Producer (TID=%d) woke up!\n", tid);
        }
        
        // Produce new value and add to buffer
        int value = i + 1;
        buffer[buffer_index] = value;
        buffer_index++;
        
        // Update tracking variables
        producer_sum += value;
        last_value = value;
        
        printf("Producer (TID=%d) produces new value successfully!: %d\n", tid, value);
        printf("Current producer sum: %d\n", producer_sum);

        // Signal consumer that data is ready
        data_ready = 1;
        pthread_cond_signal(&can_consume);
        
        printf("Producer is releasing mutex!\n\n");
        pthread_mutex_unlock(&mutex);
        
        // Sleep to simulate work
        sleep(5);
    }
    
    printf("Producer (TID=%d) finished. Total produced sum: %d\n", tid, producer_sum);
    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    pid_t pid = getpid();
    pid_t tid = gettid();
    
    printf("Consumer started: PID=%d, TID=%d\n", pid, tid);
    
    // Consume NUM_ITEMS items
    for (int i = 0; i < NUM_ITEMS; i++) {
        // Lock mutex before accessing shared resources
        pthread_mutex_lock(&mutex);
        
        printf("\nConsumer is holding mutex\n");
        
        // Wait while buffer is empty or data not ready
        while (buffer_index <= 0 || !data_ready) {
            printf("No data available. Consumer (TID=%d) is waiting...\n", tid);
            pthread_cond_wait(&can_consume, &mutex);
            printf("Consumer (TID=%d) woke up!\n", tid);
        }
        
        // Consume value from buffer
        buffer_index--;
        int value = buffer[buffer_index];
        
        // Update tracking variables
        consumer_sum += value;
        last_value = value;
        
        printf("Consumer (TID=%d) consumes a value successfully!: %d\n", tid, value);
        printf("Current consumer sum: %d\n", consumer_sum);
        
        // Signal producer that space is available
        pthread_cond_signal(&can_produce);
        
        printf("Consumer is releasing mutex\n\n");
        pthread_mutex_unlock(&mutex);
        
        // Sleep to simulate work
        sleep(10);
    }
    
    printf("Consumer (TID=%d) finished. Total consumed sum: %d\n", tid, consumer_sum);
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;
    pid_t main_pid = getpid();
    pid_t main_tid = gettid();
    
    // Initialize synchronization primitives
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&can_produce, NULL);
    pthread_cond_init(&can_consume, NULL);
    
    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    
    // Wait for threads to complete
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    
    // Cleanup synchronization primitives
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&can_produce);
    pthread_cond_destroy(&can_consume);
    
    return 0;
}
