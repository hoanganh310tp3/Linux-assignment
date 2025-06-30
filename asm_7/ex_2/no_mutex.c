#include <pthread.h>
#include <stdio.h>

int counter = 0;

void* increase(void* arg) {
	for(int i=0; i < 100000; i++){
		counter ++;
	}
	
	return NULL;
}

int main(){
	pthread_t t1, t2;

	pthread_create(&t1, NULL, increase, NULL);
	pthread_create(&t2, NULL, increase, NULL);

	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("global variable after 2 threads no lock mechanism: %d.\n",counter);

	return 0;
}



