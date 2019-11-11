#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
/*#define SHOWHOST*/
void showtime(long);
void show_info(struct utmp*);

int main()
{
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
    {
        perror(UTMP_FILE);
        exit(1);
    }
    while (read(utmpfd, &current_record, reclen)== reclen)
        show_info(&current_record);
    close(utmpfd);
    return 0;
}

void show_info(struct utmp* utbufp)
{
    if(utbufp->ut_type != USER_PROCESS)
        return;

    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    showtime(utbufp->ut_time);
#ifdef SHOWHOST
    if(utbufp->ut_host[0] != '\0')  //'\0' NULL
        printf(" (%s)", utbufp->ut_host);
#endif
    printf("\n");
}

void showtime(long timeval)
{
    char *cp;
    cp = ctime(&timeval);   //ctime으로부터 받은 배열의 첫번째를 cp가 가르키게됨

    printf("%12.12s", cp+4);    //앞에 요일은 빼고서부터 12개 문자 출력
}



