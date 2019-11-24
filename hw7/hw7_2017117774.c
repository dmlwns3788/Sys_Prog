#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define     oops(m,x)    { perror(m); exit(x); }


int new_pipe[2], old_pipe[2], pid, cur_input_idx = 0;
char *command_arr[10];
int save_out;
void middle();

int main(int ac, char *argv[])
{
    save_out = dup(1);
    if(ac < 4)    //적어도 NULL포함 3개 이상 입력되어야함
        oops("usage : pipe cmd1 | cmd2 ...\n", 1);
    
    if(strcmp(argv[ac-1], "NULL") != 0)
        oops("usage : pipe cmd1 | cmd2 ... NULL\n", 1);
    
    //함수사용을 위해서 command를 배열에 넣음
    for(int i = 1; i < ac; i++) {
        command_arr[i-1] = argv[i];
        printf("command : %s\n",command_arr[i-1]);
    }
    
    if(pipe(old_pipe) == -1)
        oops("Cannot get a pipe", 1);
    
    if((pid = fork()) == -1)
        oops("Cannot fork", 2);

    if(pid > 0) {   //first parent, first command
        close(old_pipe[0]);  //read XXX

        if(dup2(old_pipe[1], 1) == -1)   //stdout에 pipe write end 연결
            oops("Could not redirect stdin", 3);

        close(old_pipe[1]);
        execlp(command_arr[cur_input_idx], command_arr[cur_input_idx], NULL);
    }
    else {  //child
        middle();
    }
}


void middle()   //2번째부터 마지막 command 전 까지 위치한 command
{
    cur_input_idx += 1;
    printf("%s\n", command_arr[cur_input_idx]);
    
    dup2(old_pipe[0], 0);    //pipe read end와 stdin 연결
    close(old_pipe[0]);
    
    if(strcmp(command_arr[cur_input_idx+1], "NULL") == 0) {  //마지막 command라면
        close(old_pipe[1]);
        dup2(save_out, 1);
        printf("Next : %s\n", command_arr[cur_input_idx+1]);
        execlp(command_arr[cur_input_idx], command_arr[cur_input_idx], NULL);  //마지막 command 실행으로 밑에 코드들은 사라짐
    }
    
    pipe(old_pipe);
    pid = fork();

    if(pid > 0) {
        dup2(old_pipe[1], 1);   //stdout에 pipe write end 연결
        close(old_pipe[1]);
        execlp(command_arr[cur_input_idx], command_arr[cur_input_idx], NULL);
    }
    
    //printf("%d\n",getpid());
    middle();

}
