#include <stdio.h>
#include <pthread.h>

// Global variable shared between threads
int tmp = 5;

// Structure to share values between threads
typedef struct {
	int value_1;
	int value_2;
} ShareValue;

// Global instance of ShareValue struct
ShareValue global_variable;

// Thread 1 function - increments tmp by 1 and sets value_1
void* thread1_func(void* arg) {
	tmp = tmp+1;
	printf("thread 1 is created successfully!\n");
	printf("global variable in thread 1 is %d.\n", tmp);
	global_variable.value_1 = 100;
	return NULL;
}

// Thread 2 function - increments tmp by 2 and sets value_2 
void* thread2_func(void* arg) {
	tmp = tmp+2;
    printf("thread 2 is created successfully!\n");
	printf("globle variable in thread 2 is %d.\n", tmp);
	global_variable.value_2 = 200;
    return NULL;
}

int main(){
	// Declare thread identifiers
	pthread_t thread1;
	pthread_t thread2;

	// Create two threads
	pthread_create(&thread1, NULL, thread1_func, NULL);
	pthread_create(&thread2, NULL, thread2_func, NULL);

	// Wait for both threads to complete
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

	// Print the values set by each thread
	printf("Thread 1 set value %d.\n",global_variable.value_1);
	printf("Thread 2 set value %d.\n",global_variable.value_2);

	return 0;
}
