#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(void)
{
    void f(int);
    int i;
    signal(SIGINT, f);
    for(i=0; i<20; i++){
        printf("hello\n");
        sleep(1);
    }

    return 0;
}
void f(int sifnum)
{
    printf("OUCH!\n");
}
