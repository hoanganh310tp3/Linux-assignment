#include <stdio.h>
#include <pthread.h>

int tmp = 5;

typedef struct {
	int value_1;
	int value_2;
} ShareValue;

ShareValue global_variable;

void* thread1_func(void* arg) {
	tmp = tmp+1;
	printf("thread 1 is created successfully!\n");
	printf("globle variable in thread 1 is %d.\n", tmp);
	global_variable.value_1 = 100;
	return NULL;
}

void* thread2_func(void* arg) {
	tmp = tmp+2;
    printf("thread 2 is created successfully!\n");
	printf("globle variable in thread 2 is %d.\n", tmp);
	global_variable.value_2 = 200;
    return NULL;
}

int main(){
	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, NULL, thread1_func, NULL);
	pthread_create(&thread2, NULL, thread2_func, NULL);

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	printf("Thread 1 set value %d.\n",global_variable.value_1);
	printf("Thread 2 set value %d.\n",global_variable.value_2);

	return 0;
}

