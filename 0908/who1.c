#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define SHOWHOST
void show_info(struct utmp*);

int main()
{
    struct utmp current_record; //utmp struct형인 current_record 변수 생성
    int utmpfd;
    int reclen = sizeof(current_record);    //읽어들일 size 정의

    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)  //how = ReadOnly
    {
        perror(UTMP_FILE);
        exit(1);
    }
    while (read(utmpfd, &current_record, reclen) == reclen) //하나 읽고 하나 출력을 반복,  성공 시 읽어들인 size를 리턴하므로 reclen과 같다면 성공적으로 읽은 것
        show_info(&current_record);

    close(utmpfd);

    return 0;
}

void show_info(struct utmp* utbufp)
{
    printf("%-8.8s", utbufp->ut_name);
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);
    printf(" ");
    printf("%101d", utbufp->ut_time);
    printf(" ");
#ifdef SHOWHOST     //맨 처음 한 번만 실행됨
    printf("(%s)", utbufp->ut_host);
#endif
    printf("\n");
}

