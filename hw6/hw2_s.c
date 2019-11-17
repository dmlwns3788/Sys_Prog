#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#define BUFFERSIZE	200

unsigned array[20000];
struct timeval start_time, end_time;
void copyFile(int *, int);

unsigned int ascending(const void *a, const void *b){
    unsigned int ia = *(unsigned int *)a;
    unsigned int ib = *(unsigned int *)b;
    return (ascending_array[ia] > ascending_array[ib]) - (ascending_array[ia] < ascending_array[ib]);
}


unsigned int descending(const void *a, const void *b){
    unsigned int ia = *(unsigned int *)a;
    unsigned int ib = *(unsigned int *)b;
    return (decending_array[ia] < decending_array[ib]) - (decending_array[ia] > decending_array[ib]);
}


int main(int argc, char* argv[])	//argv[1] : mode
{

	if(argc == 1)
		perror("Select mode");

	unsigned int ascending_array[20000];
	unsigned int decending_array[20000];
	unsigned int child_pid[2];
	int fd1, fd2, fd3;

	//create file
	if(fd1 = creat("random_number.txt", 0644) == -1) {
		fprintf(stderr, "creat error");
		exit(1);
	}
	if(fd2 = creat("ascending_number.txt", 0644) == -1) {
		fprintf(stderr, "creat error");
		exit(1);
	}
	if(fd3 = creat("descending_number.txt", 0644) == -1) {
		fprintf(stderr, "creat error");
		exit(1);
	}
	fd1 = open("random.txt", O_WRONLY);
	fd2 = open("ascending_number.txt", O_WRONLY);
	fd3 = open("descending_number.txt", O_WRONLY);

	srand(time(NULL));

	for (unsigned int i=0; i<20000;i++) {
		array[i] = rand()%20000;
		ascending_array[i] = array[i];
		decending_array[i] = array[i];
	}
    
    copyFile(array, fd1);

	if ((strcmp(argv[1] , "0") == 0)) {
		gettimeofday(&start_time, NULL);

		//execute the four tasks in parallel using fork()
		child_pid[0] = fork();
		printf("%d\n", child_pid[0]);
		if(child_pid[0] == 0) {
			printf("first chld! \n");
			printf("first parent :%d\n",getppid());
			qsort(ascending_array, sizeof(ascending_array)/sizeof(int), sizeof(int), ascending);
			//FPRINT
            copyFile(array, fd2);
			exit(1);	//child process 종료
		}
		else
			child_pid[1] = fork();
		printf("%d\n", child_pid[1]);
		if(child_pid[1] == 0) {
			printf("second child!\n");
			printf("second parent : %d\n", getppid());
			qsort(decending_array, sizeof(decending_array)/sizeof(int), sizeof(int), descending);
			//FPRINT
            copyFile(array, fd3);
			exit(1);
		}
			
        gettimeofday(&end_time, NULL);
        
        double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
        printf("Elapsed: %f seconds\n", operating_time);
		
	}
	else {
		gettimeofday(&start_time, NULL);

		//execute the four tasks in sequential
		qsort(ascending_array, sizeof(ascending_array)/sizeof(int), sizeof(int), ascending);
		qsort(decending_array, sizeof(decending_array)/sizeof(int), sizeof(int), descending);
		
        copyFile(array, fd2);
        copyFile(array, fd3);

		gettimeofday(&end_time, NULL);
		
		double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
			printf("Elapsed: %f seconds\n", operating_time);
	}	
}

/**
 file 생성 후 배열의 데이터를 파일에 복사하는 함수
 **/
void copyFile(int *arr, int e_fd)
{
    int i;
    
	for(i=0; i < 20000; i++) {
        fprintf(e_fd, "%d\t", arr[i]);
        if(i % 10 == 9)
            fprintf(e_fd, "\n");
    }
    
    
    /*
    char buffer[200];
	int i;

	for(i=0; i < 20000; i++) {
		if(i % 200 == 0)
            sprintf(buffer, "%d \n", arr[i]);
        else
            sprintf(buffer, "%d ", arr[i])
        write(e_fd, buffer, strlen(buffer));
	}
     */
}
