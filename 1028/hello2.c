#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main()
{
    int i;

    initscr();
    clear();
    for(i=0; i<LINES; i++){
        move(i, i+i);
        printf("%d = %d =\n", i,i%2);
        if(i%2 == 1)
            standout();
        addstr("Hello, world");
        if(i%2 == 1)
            standend();
    }
    refresh();
    sleep(3);
    endwin();

    return 0;
}
