#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_spinlock_t spinlock;
int counter = 0;
#define INCREMENTS_PER_THREAD 100

typedef struct {
	int thread_id;
} thread_data_t;

void *increment(void *arg)
{
	thread_data_t *data = (thread_data_t *) arg;

    /*Tight loop*/	
	for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
		pthread_spin_lock(&spinlock);
		counter++;
		pthread_spin_unlock(&spinlock);
	}
	
	printf("Thread %d completed %d increments\n", 
		   data->thread_id, INCREMENTS_PER_THREAD);
	
	return NULL;
}

int main()
{
	int n;
	printf("Please insert number of threads: ");
	scanf("%d", &n);

	pthread_t threads[n];
	thread_data_t thread_data[n];

	pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

	for (int i = 0; i < n; i++) {
		thread_data[i].thread_id = i + 1;
	}

	for (int i = 0; i < n; i++) {
		pthread_create(&threads[i], NULL, increment,
					   &thread_data[i]);
	}

	for (int i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_spin_destroy(&spinlock);

	printf("Final counter value: %d\n", counter);
	return 0;
}
