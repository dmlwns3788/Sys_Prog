#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define ARR_SIZE    20000

unsigned array[ARR_SIZE];
struct timeval start_time, end_time;
void bubble_sort(unsigned int [], int, int);
void copyFile(unsigned int [], FILE *);


int main(int argc, char *argv[])
{
    unsigned int ascending_array[ARR_SIZE];
    unsigned int decending_array[ARR_SIZE];
    int child_pid[2];
    
    FILE * fp1 = fopen("random_number.txt","wt");
    FILE * fp2 = fopen("ascending_number.txt","wt");
    FILE * fp3 = fopen("decending_number.txt","wt");

    srand(time(NULL));
    
    for(int i = 0; i < ARR_SIZE; i++) {
        array[i] = rand() % 20000;
        ascending_array[i] = array[i];
        decending_array[i] = array[i];
    }
    
    copyFile(array, fp1);   //print file contents
    
    if((strcmp(argv[1], "0") == 0)) {
        gettimeofday(&start_time, NULL);    //start timeer
        
        child_pid[0] = fork();
        
        if(child_pid[0] > 0)    //부모라면
            child_pid[1] = fork();
        else {  //자식 프로세스
            bubble_sort(ascending_array, ARR_SIZE, 0);
            copyFile(ascending_array, fp2);
            exit(1);
        }
        if(child_pid[1] == 0) { //자식 프로세스
            bubble_sort(decending_array, ARR_SIZE, 1);
            copyFile(decending_array, fp3);
            exit(1);
        }
        wait(NULL);
        wait(NULL);
        
        gettimeofday(&end_time, NULL);
        double operating_time = (double)(end_time.tv_sec)+(double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec)+(double)(start_time.tv_usec)/1000000.0;
        
        printf("Elapsed: %f seconds\n", (double)operating_time);
        
    }
    else {  //sequential
        gettimeofday(&start_time, NULL);

        //execute the four tasks in sequential
        bubble_sort(ascending_array, ARR_SIZE, 0);
        bubble_sort(decending_array, ARR_SIZE, 1);
        
        copyFile(ascending_array, fp2);
        copyFile(decending_array, fp3);

        gettimeofday(&end_time, NULL);
        double operating_time = (double)(end_time.tv_sec) + (double)(end_time.tv_usec)/1000000.0 - (double)(start_time.tv_sec) + (double)(start_time.tv_usec)/1000000.0;
        
        printf("Elapsed: %f seconds\n", operating_time);
    }
    
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    
    return 0;
}


void bubble_sort(unsigned int arr[], int count, int mode)
{
    int temp;

    if(mode == 0) //acending mode
    {
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < count - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }
    else {
        for (int i = 0; i < count; i++) {
                 for (int j = 0; j < count - 1; j++) {
                     if (arr[j] < arr[j + 1]) {
                         temp = arr[j];
                         arr[j] = arr[j + 1];
                         arr[j + 1] = temp;
                     }
                 }
             }
    }
}


void copyFile(unsigned int arr[], FILE *fp)
{
    for(int k = 0; k < ARR_SIZE; k++) {
        fprintf(fp, "%8d", arr[k]);
        if(k % 10 == 9)
            fprintf(fp, "\n");
    }
}
