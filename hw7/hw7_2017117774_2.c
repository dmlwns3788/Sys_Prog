#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define     oops(m,x)    { perror(m); exit(x); }


char **command_arr[10];

void middle();

int main(int ac, char *argv[])
{
    int new_pipe[2], old_pipe[2], pid, cur_input_idx = 1;

    if(ac < 4)    //적어도 NULL포함 3개 이상 입력되어야함
        oops("usage : pipe cmd1 | cmd2 ...\n", 1);
    
    if(strcmp(argv[ac-1], "NULL") != 0)
        oops("usage : pipe cmd1 | cmd2 ... NULL\n", 1);
    
    if(pipe(old_pipe) == -1)
        oops("Cannot get a pipe", 1);
    
    if((pid = fork()) == -1)
        oops("Cannot fork", 2);

    if(pid > 0) {   //first parent, first command
        close(old_pipe[0]);  //read XXX

        if(dup2(old_pipe[1], 1) == -1)   //stdout에 pipe write end 연결
            oops("Could not redirect stdin", 3);

        close(old_pipe[1]);
        execlp(argv[cur_input_idx], argv[cur_input_idx], NULL);
    }
    else {  //child
        cur_input_idx++;
        
        while(strcmp(argv[cur_input_idx], "NULL") != 0) {
            printf("%s\n",argv[cur_input_idx]);
            pipe(new_pipe);
            pid = fork();
            
            if(pid > 0) {
                dup2(old_pipe[0], 0);    //pipe read end와 stdin 연결
                close(old_pipe[0]);
                
                if(strcmp(argv[cur_input_idx+1], "NULL") != 0) {
                    dup2(new_pipe[1], 1);
                    close(new_pipe[1]);
                }
                execlp(argv[cur_input_idx], argv[cur_input_idx], NULL);
                oops("execlp error in while", 4);
            }
            
            for(int k = 0; k<2; k++)
                old_pipe[k] = new_pipe[k];
            
            cur_input_idx++;
        }
    }
}

