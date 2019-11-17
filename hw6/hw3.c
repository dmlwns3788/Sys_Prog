#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#define BUFFERSIZE	4096

unsigned array[50000];
struct timeval start_time, end_time;

unsigned int ascending(const void *a, const void *b){
    unsigned int ia = *(unsigned int *)a;
    unsigned int ib = *(unsigned int *)b;
    return (array[ia] > array[ib]) - (array[ia] < array[ib]);
}


unsigned int descending(const void *a, const void *b){
    unsigned int ia = *(unsigned int *)a;
    unsigned int ib = *(unsigned int *)b;
    return (array[ia] < array[ib]) - (array[ia] > array[ib]);
}


int main(int argc, char* argv[])	//argv[1] : mode
{

	if(argc == 1)
		perror("Select mode");

	unsigned int ascending_array_idx[50000];
	unsigned int decending_array_idx[50000];
	unsigned int child_pid[2];
	child_pid[0] = 1;
	child_pid[1] = 1;


	srand(time(NULL));

	for (unsigned int i=0; i<50000;i++) {
		array[i] =rand()%50000;
		ascending_array_idx[i] = i;
		decending_array_idx[i] = i;
	}

	
	/**
	for (unsigned int i=0; i<20000;i++){
		printf("%d\n",array[ascending_array_idx[i]]);
	}
	**/
	if ((strcmp(argv[1] , "0") == 0)) {
		gettimeofday(&start_time, NULL);

		//execute the four tasks in parallel using fork()
		child_pid[0] = fork();
		printf("%d\n", child_pid[0]);
		if(child_pid[0] == 0) {
			printf("first chld! \n");
			printf("first parent :%d\n",getppid());
			qsort(ascending_array_idx, sizeof(ascending_array_idx)/sizeof(int), sizeof(int), ascending);
			//FPRINT
			exit(1);	//child process 종료
		}
		else
			child_pid[1] = fork();
		printf("%d\n", child_pid[1]);
		if(child_pid[1] == 0) {
			printf("second child!\n");
			printf("second parent : %d\n", getppid());
			qsort(ascending_array_idx, sizeof(ascending_array_idx)/sizeof(int), sizeof(int), descending);
			//FPRINT
			exit(1);
		}
		int ch1 = wait(NULL);
		printf("1 : %d\n", ch1);
		int ch2 = wait(NULL);
		printf("2 :%d\n", ch2);
			
		gettimeofday(&end_time, NULL);
			
			double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
			printf("Elapsed: %f seconds\n", operating_time);
		

		gettimeofday(&start_time, NULL);

		//execute the four tasks in sequential
		qsort(ascending_array_idx, sizeof(ascending_array_idx)/sizeof(int), sizeof(int), ascending);
		qsort(ascending_array_idx, sizeof(ascending_array_idx)/sizeof(int), sizeof(int), descending);
		
		//fprintf() 1
		//fprintf() 2

		gettimeofday(&end_time, NULL);
			operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
		
		printf("Elapsed: %f seconds\n", operating_time);
}
}

/**
 file 생성 후 배열의 데이터를 파일에 복사하는 함수
 **/
void cretefile(int *arr, int arr_len)
{
	int buffer[20000];
	
}
