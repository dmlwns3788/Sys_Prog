#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#define COMMAND_MODE 111
#define INSERT_MODE 222
void set_cr_noecho_mode(void);
int mode_ver();
void change_echo_mode(int);
void tty_mode(int);
void rotate_char(int);

int main()
{
    int response;

    tty_mode(0);            /*백업*/
    set_cr_noecho_mode();   /*canonical mode and echo mode off*/
    
    while(1){
        mode_ver();
    }
}
int mode_ver()
{
    static short int mode_bit = COMMAND_MODE;
    int c;

    if((c = getchar()) == EOF){
        fprintf(stderr,"no input data\n");
        exit(1);
    }

    if(mode_bit == COMMAND_MODE) {
        if('i' == c){
            printf("\ninsert mode!\n");
            mode_bit = INSERT_MODE;
        }else if('1' == c) {
            printf("\necho on!\n");
            change_echo_mode(1);
        }else if('2' == c) {
            printf("\necho off!\n");
            change_echo_mode(0);
        }else if('q' == c) {
            tty_mode(1);    //restore
            exit(0);
        }
    }else if(mode_bit == INSERT_MODE) {
        if(27 == c) {
            printf("\ncommand mode!\n");
            mode_bit = COMMAND_MODE;
        }
        else
            rotate_char(c);
    }
}
void rotate_char(int c)
{
    if(c == 'z')
        c = 'a';
    else if(islower(c))
        c++;
    putchar(c);
}
void tty_mode(int how)
{
    static struct termios original_mode;        /*terminal's original mode back up, 메모리에 남겨두기 위해서 static으로 선언*/
    static int original_flags;                  /*device attributes back up*/
    static int stored = 0;

    if(how == 0){
        tcgetattr(0, &original_mode);           /*원래 터미널 설정 백업*/
        original_flags = fcntl(0, F_GETFL);     /*fp=0, input device driver 설정 백업*/
        stored = 1;
    }else if(stored){
        tcsetattr(0, TCSANOW, &original_mode);              /*복구*/
        original_flags = fcntl(0, F_SETFL, original_flags); /*복구*/
    }
}
void set_cr_noecho_mode(void)
{
    struct termios ttystate;

    tcgetattr(0, &ttystate);
    ttystate.c_lflag &= ~ICANON;        /*icanon off(buffering off, not using enter)*/
    ttystate.c_lflag &= ~ECHO;          /*echo off*/
    ttystate.c_cc[VMIN] = 1;            /*key 한 번만 누르면 바로 프로그램으로 전달*/
    tcsetattr(0, TCSANOW, &ttystate);
}
void change_echo_mode(int how)
{
    struct termios ttystate;
    
    tcgetattr(0, &ttystate);
    if(0 == how)                        /*echo off*/
        ttystate.c_lflag &= ~ECHO;
    else if(1 == how)                   /*echo on*/
        ttystate.c_lflag |= ECHO;
    tcsetattr(0, TCSANOW, &ttystate);
}
