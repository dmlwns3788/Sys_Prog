#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define BUFFERSIZE	4096

unsigned array[20000];
struct timeval start_time, end_time;

unsigned int ascending(const void *a, const void *b){
    unsigned int ia = *(unsigned int *)a;
    unsigned int ib = *(unsigned int *)b;
    return (ascending_array[ia] > ascending_array[ib]) - (ascending_array[ia] < ascending_array[ib]);
}


unsigned int descending(const void *a, const void *b){
    unsigned int ia = *(unsigned int *)a;
    unsigned int ib = *(unsigned int *)b;
    return (descening_array[ia] < descending_array[ib]) - (descending_array[ia] > descending_array[ib]);
}


int main(int argc, char* argv[])	//argv[1] : mode
{

	if(argc == 1)
		perror("Select mode");

	unsigned int ascending_array[20000];
	unsigned int decending_array[20000];
	unsigned int child_pid[2];

	srand(time(NULL));

	for (unsigned int i=0; i<20000;i++) {
		array[i] =rand()%20000;
		ascending_array[i] = array[i];
		decending_array[i] = array[i];
	}

	if ((strcmp(argv[1] , "0") == 0)) {
		gettimeofday(&start_time, NULL);

		//execute the four tasks in parallel using fork()
		child_pid[0] = fork();

		if(child_pid[0] == 0) {
			qsort(ascending_array, sizeof(ascending_array)/sizeof(int), sizeof(int), ascending);
			exit(1);	//child process 종료
		}
		else
			child_pid[1] = fork();
		
		if(child_pid[1] == 0) {
				qsort(descending_array, sizeof(descending_array)/sizeof(int), sizeof(int), descending);
				exit(1);
		}
		else {
			while(wait(NULL) != child_pid[1])
				;
			gettimeofday(&end_time, NULL);
			double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) - (double)(start_time.tv_usec)/1000000.0;
			printf("Elapsed: %f seconds\n", operating_time);
		}
	}
	else {
		gettimeofday(&start_time, NULL);
		//execute the four tasks in sequential
		qsort(ascending_array_idx, sizeof(ascending_array_idx)/sizeof(int), sizeof(int), ascending);
		qsort(ascending_array_idx, sizeof(ascending_array_idx)/sizeof(int), sizeof(int), descending);
		gettimeofday(&end_time, NULL);
		double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) - (double)(start_time.tv_usec)/1000000.0;
			printf("Elapsed: %f seconds\n", operating_time);
		//fprintf()
		//fprintf()
	}	
}
