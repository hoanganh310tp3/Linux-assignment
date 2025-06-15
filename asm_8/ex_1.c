#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t lock;
int counter = 0;

typedef struct {
	int thread_id;
	int sleep_time;
} thread_data_t;

void *increment(void *arg)
{
	thread_data_t *data = (thread_data_t *) arg;
	sleep(data->sleep_time);
	pthread_mutex_lock(&lock);
	counter++;
	printf("Thread %d incremented counter to %d\n", data->thread_id,
	       counter);
	pthread_mutex_unlock(&lock);

	return NULL;
}

int main()
{
	int n;
	printf("Please insert number of threads: ");
	scanf("%d", &n);

	pthread_t threads[n];
	thread_data_t thread_data[n];

	pthread_mutex_init(&lock, NULL);


	srand(time(NULL));


	for (int i = 0; i < n; i++) {
		thread_data[i].thread_id = i + 1;
		thread_data[i].sleep_time = (rand() % 3) + 1;
	}

	for (int i = 0; i < n; i++) {
		pthread_create(&threads[i], NULL, increment,
			       &thread_data[i]);
	}

	for (int i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_mutex_destroy(&lock);

	printf
	    ("Final counter value after %d threads with mutex protection: %d\n",
	     n, counter);


	return 0;
}
