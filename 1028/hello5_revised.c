#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <termio.h>
#define LEFTEDGE 10
#define RIGHTEDGE 30
#define ROW 10
int set_ticker(int);
void cc();
int pos = LEFTEDGE;

int main(int ac, char *av[])
{
    int time;
    int in_ch;
    char message[] = "Hello";

    if(ac == 1)
        printf("insert a input value");
    else
        time = atoi(av[1]);         /*문자500 을 정수 500으로 변환*/

    signal(SIGALRM, cc);            /*SIGALRM을 받으면 cc를 호출*/
    if(set_ticker(time) == -1)      /*timer설정*/
        perror("set_ticker");
    
    initscr();                      /*curses모드를 시작*/
    clear();                        /*화면 비움*/
    nodelay(stdscr, TRUE);          /*getch() 호출 후 입력을 기다리지 않음*/
    noecho();                       /*입력받은 문자를 화면에 출력하지 않음*/

    while(1){
        in_ch = getch();            /*getchar()처럼 개행 문자를 기다리지 앟고 입력받은 문자를 바로 리턴해줌*/
        if(in_ch == 'p'){
            time+=50;
            set_ticker(time);
        }else if(in_ch == 'm'){
            time-=50;
            set_ticker(time);
        }else if(in_ch == 'q'){     
            endwin();               /*curses 모드를 종료*/
            exit(0);                /*프로그램 종료*/
        }
        move(ROW, pos);             /*curser를 이동*/
        addstr(message);            /*stdscr 버퍼에 업데이트된 screen정보를 담는다*/
        move(LINES-1, COLS-1);      /*커서를 잘 안보이게 화면 구석으로 이동*/
        refresh();                  /*stdscr 버퍼와 curscr 버퍼를 비교해서 새로 update된 부분만 curscr에 update한다 => 데이터 전송을 줄이기 위해서 두 버퍼를 사용*/
        pause();                    /*process정지, SET_TICKER()로 설정한 SIGALRM을 받을 때까지*/
    }
    return 0;
}

void cc(int signum)
{
    char blank[] = "       ";
    static int dir = +1;

    move(ROW, pos);             /*curser이동*/
    addstr(blank);              /*해당 curser 위치에 문자열 삽입*/
    pos += dir;                 /*전역변수 pos를 수정*/
    if(pos >= RIGHTEDGE)        /*오른쪽 끝에 도달했을 때 dir을 반대 방향으로 설정*/
        dir = -1;
    if(pos <= LEFTEDGE)         /*왼쪽 끝에 도달했을 때 dir을 반대 방향으로 설정*/
        dir = +1;
}

int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;
 
    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;
  
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
