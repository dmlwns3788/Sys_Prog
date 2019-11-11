#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#define ASK    "Do you want another transaction"
#define TRIES 3
#define SLEEPTIME 2
#define BEEP putchar('\a')
int get_response(char *, int);
void set_cr_noecho_mode(void);
void tty_mode(int);
void set_nodelay_mode(void);
int get_ok_char(void);

int main()
{
    int response;

    tty_mode(0);                                    /*backup*/
    set_cr_noecho_mode();                           /*canon 변경*/
    set_nodelay_mode();
    get_ok_char();
    response = get_response(ASK, TRIES);
    tty_mode(1);                            /*restore*/
    return response;
}
int get_response(char *question, int maxtries)
{
    int input;
    printf("%s (y/n)?", question);      /*dirver buffer는 read시도가 있을 때까지 output버퍼는 대기하고 있기 때문에 수동으로 비워줘야 원하는 때에 출력 가능하다*/
    fflush(stdout);
    while(1){
        sleep(SLEEPTIME);
        input = tolower(get_ok_char());
        if(input == 'y')
            return 0;
        if(input =='n')
            return 1;
        if(maxtries-- == 0)             /*3번 시도*/
            return 2;
        BEEP;
    }
}
int get_ok_char(void)
{
    int c;
    while( ( (c = getchar()) != EOF) && strchr("yYnN",c) == NULL)
        printf("%c",c);;
    return 0;
}
void set_nodelay_mode(void)
{
    int termflags;
    termflags = fcntl(0, F_GETFL);
    termflags |= O_NDELAY;
    fcntl(0, F_SETFL, termflags);
}
void set_cr_noecho_mode(void)
{
    struct termios ttystate;

    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;                /*off, 버퍼링 없이(enter없이) 누르는 input을 바로 전송*/
    ttystate.c_lflag &= ~ECHO;                  /*off*/
    ttystate.c_cc[VMIN] = 1;                    /*key한 번 누르면 입력됨,  2면 두 번 눌러야 입력됨*/
    tcsetattr(0, TCSANOW, &ttystate);           /*input device의 fd(=0)의 설정을 변경하는 것*/
}
void tty_mode(int how)
{
    static struct termios original_mode;        /*백업할 mode가 계속 메모리에 남아있도록 static으로 선언함*/
    static int original_flags;

    if(how == 0){
        tcgetattr(0, &original_mode);           /*원래 mode 백업*/
        original_flags = fcntl(0, F_GETFL);
    }
    else{
        tcsetattr(0, TCSANOW, &original_mode);  /*원래 mode로 복구*/
        original_flags = fcntl(0, F_SETFL, original_flags);
    }
}
