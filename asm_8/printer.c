#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

sem_t printer_sem;
int available_printers[3] = {1, 2, 3};
int printer_index = 0;
pthread_mutex_t printer_mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int thread_id;
    int print_time;
} thread_data_t;

int get_printer() {
    pthread_mutex_lock(&printer_mutex);
    int printer = available_printers[printer_index];
    printer_index = (printer_index + 1) % 3;
    pthread_mutex_unlock(&printer_mutex);
    return printer;
}

void* thread_func(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    printf("Thread %d is waiting for a printer...\n", data->thread_id);
    
    sem_wait(&printer_sem);
    
    int printer_id = get_printer();
    
    printf("Thread %d got Printer %d and started printing\n", data->thread_id, printer_id);
    
    sleep(data->print_time);
    
    printf("Thread %d finished printing on Printer %d\n", data->thread_id, printer_id);
    
    sem_post(&printer_sem);
    
    return NULL;
}

int main() {
    pthread_t threads[15];
    thread_data_t thread_data[15];
    
    sem_init(&printer_sem, 0, 3);
    
    srand(time(NULL));
    
    for (int i = 0; i < 15; i++) {
        thread_data[i].thread_id = i + 1;
        thread_data[i].print_time = (rand() % 3) + 1;
    }
    
    
    for (int i = 0; i < 15; i++) {
        pthread_create(&threads[i], NULL, thread_func, &thread_data[i]);
    }
    
    for (int i = 0; i < 15; i++) {
        pthread_join(threads[i], NULL);
    }
    
    sem_destroy(&printer_sem);
    pthread_mutex_destroy(&printer_mutex);
    
    return 0;
}