#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFERSIZE    200

unsigned array[20000];
unsigned int ascending_array[20000];
unsigned int decending_array[20000];
struct timeval start_time, end_time;
void copyFile(int [], int);


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


int main(int argc, char* argv[])    //argv[1] : mode
{
    if(argc < 2)
        perror("Select mode");

    unsigned int child_pid[2];
    child_pid[0] = 1;
    child_pid[1] = 1;
    int fd1, fd2, fd3;
    
    //create file
    
    if(fd1 == creat("random_number.txt", 0644) == -1) {
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
     
    
    fd1 = open("random_number.txt", O_WRONLY);
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
        if(child_pid[0] == 0) {
            qsort(ascending_array, sizeof(ascending_array)/sizeof(int), sizeof(int), ascending);
            //FPRINT
            copyFile(ascending_array, fd2);
            exit(1);    //child process 종료
        }
        else
            child_pid[1] = fork();
        
        if(child_pid[1] == 0) {
            qsort(decending_array, sizeof(decending_array)/sizeof(int), sizeof(int), descending);
            //FPRINT
            copyFile(decending_array, fd3);
            exit(1);
        }
        int ch1 = wait(NULL);
        //printf("1 : %d\n", ch1);
        int ch2 = wait(NULL);
        //printf("2 :%d\n", ch2);
            
        gettimeofday(&end_time, NULL);
            
        double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
        printf("Elapsed: %f seconds\n", operating_time);
    }
    else {
        gettimeofday(&start_time, NULL);

        //execute the four tasks in sequential
        qsort(ascending_array, sizeof(ascending_array)/sizeof(int), sizeof(int), ascending);
        qsort(decending_array, sizeof(decending_array)/sizeof(int), sizeof(int), descending);
        
        copyFile(ascending_array, fd2);
        copyFile(decending_array, fd3);

        gettimeofday(&end_time, NULL);
        double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
        
        printf("Elapsed: %f seconds\n", operating_time);
    }
    
    close(fd1);
    close(fd2);
    close(fd3);
}

void copyFile(int arr[], int fd)
{
    int i;
    char buffer[100];

    for(i=0; i< 20000; i++) {
        if(i % 10 == 0 && i != 0)
            sprintf(buffer, "%d \t", arr[i]);
        else
            sprintf(buffer, "\n");
    write(fd, arr, strlen(buffer));
    }
}
