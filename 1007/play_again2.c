#include <stdio.h>
#include <termios.h>

#define QUESTION    "Do you want another transaction"
int get_response(char *);
void set_crmode(void);
void tty_mode(int);

int main()
{
    int response;
    tty_mode(0);                            /*backup*/
    set_crmode();                           /*canon 변경*/
    response = get_response(QUESTION);
    tty_mode(1);                            /*restore*/
    return response;
}
int get_response(char *question)
{
    int input;
    printf("%s (y/n)?", question);
    while(1){
        switch(input = getchar()) {
            case 'y':
            case 'Y': return 0;
            case 'n':
            case 'N':
            case EOF: return 1;
        }
    }
}
void set_crmode(void)
{
    struct termios ttystate;

    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;                /*off, 버퍼링 없이(enter없이) 누르는 input을 바로 전송*/
    ttystate.c_lflag &= ~ECHO;                  /*off*/
    ttystate.c_cc[VMIN] = 1;                    /*key한 번 누르면 입력됨,  2면 두 번 눌러야 입력됨*/

    tcsetattr(0, TCSANOW, &ttystate);
}
void tty_mode(int how)
{
    static struct termios original_mode;        /*백업할 mode가 계속 메모리에 남아있도록 static으로 선언함*/
    if(how == 0)  //
        tcgetattr(0, &original_mode);           /*원래 mode 백업*/
    else
        tcsetattr(0, TCSANOW, &original_mode);  /*원래 mode로 복구*/
}
