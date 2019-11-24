#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define     oops(m,x)    { perror(m); exit(x); }


int pipe_fd[2], cur_input_idx = 0;
pid_t pid;
char *command_arr[100];

void middle();

int main(int ac, char *argv[])
{
    if(ac < 4)    //적어도 NULL포함 3개 이상 입력되어야함
        oops("usage : pipe cmd1 | cmd2 ...\n", 1);
    
    if(strcmp(argv[ac-1], "NULL") != 0)
        oops("usage : pipe cmd1 | cmd2 ... NULL\n", 1);
    
    //함수사용을 위해서 command를 배열에 넣음
    for(int i = 1; i < ac; i++)
        command_arr[i-1] = argv[i];
    
    if(pipe(pipe_fd) == -1)
        oops("Cannot get a pipe", 1);
    
    if((pid = fork()) == -1)
        oops("Cannot fork", 2);

    if(pid == 0) {   //child, firts command
        close(pipe_fd[0]);      //read XXX

        if(dup2(pipe_fd[1], 1) == -1)   //stdout에 pipe write end 연결
            oops("Could not redirect stdin", 3);

        close(pipe_fd[1]);
        execlp(command_arr[cur_input_idx], command_arr[cur_input_idx], NULL);
    }
    else {  //parent
        middle();
    }
}


void middle()   //2번째부터 마지막 command 전 까지 위치한 command
{
    cur_input_idx += 1;
    
    dup2(pipe_fd[0], 0);    //pipe read end와 stdin 연결
    close(pipe_fd[0]);      //dup2는 close후에 복사가 이뤄짐
    close(pipe_fd[1]);

    
    if(strcmp(command_arr[cur_input_idx+1], "NULL") == 0) {  //마지막 command라면
        //parent의 stdout은 여전히 화면 출력으로 설정되어 있으므로 다른 조치를 할 필요 없음
        execlp(command_arr[cur_input_idx], command_arr[cur_input_idx], NULL);  //마지막 command 실행으로 밑에 코드들은 사라짐
    }
    
    pipe(pipe_fd);      //새로운 pipe 생성
    pid = fork();

    if(pid == 0) {  //child
        dup2(pipe_fd[1], 1);   //stdout에 pipe write end 연결
        close(pipe_fd[1]);
        close(pipe_fd[0]);
        execlp(command_arr[cur_input_idx], command_arr[cur_input_idx], NULL);
    }
    
    middle();   //새로 받은 pipe를 가진 부모가 다시 middle() 실행

}
