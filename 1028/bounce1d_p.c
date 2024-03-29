/* bounce1d.c
 *	purpose	animation with user controlled speed and direction
 *	note	the handler does the animation
 *		the main program reads keyboard input
 *	compile	cc bounce1d.c set_ticker.c -lcurses -o bounce1d
 */

#include  <stdio.h>
#include  <curses.h>
#include  <signal.h>
#include  <string.h>
#include  <termios.h>
#include        <sys/time.h>
#include        <stdlib.h>


/* some global settings main and the handler use */

#define	MESSAGE	"hello"
#define BLANK   "     "
int	row;	/* current row		*/
int	col;	/* current column	*/
int	dir;	/* where we are going	*/

void set_cr_noecho_mode(void);
int set_ticker(int);   		


int main(void)
{
	int	delay;		/* bigger => slower	*/
	int	ndelay;		/* new delay		*/
	int	c;		/* user input		*/
	void	move_msg(int);	/* handler for timer	*/

    initscr();
	set_cr_noecho_mode();

	clear();
	
	row   = 10;		/* start here		*/
	col   = 0;
	dir   = 1;		/* add 1 to row number	*/
	delay = 200;		/* 200ms = 0.2 seconds  */

	move(row,col);		/* get into position	*/
	addstr(MESSAGE);	/* draw message		*/
	signal(SIGALRM, move_msg );
	set_ticker( delay );

	while(1)
	{
        //todo;
        ndelay =0;
        c = getchar();
        if( c == 'Q' ) break;
        if( c == ' ' ) dir = -dir;
        if( c == 'f' && delay >2 ) ndelay = delay/2;
        if( c == 's' ) ndelay = delay * 2;
        if( ndelay > 0 ) set_ticker(delay = ndelay);
	}

	endwin();

	return 0;
}

void move_msg(int signum)
{
    //todo;
    signal(SIGALRM, move_msg);  /* reset, just in case */
    move(row, col);
    addstr(BLANK);
    col += dir;
    move(row, col);
    addstr(MESSAGE);
    refresh();

    if(dir == -1 && col <= 0)
        dir = 1;
    else if(dir == 1 && col+strlen(MESSAGE) >= COLS)
        dir = -1;
}

void set_cr_noecho_mode(void)
/*
 * purpose: put file descriptor 0 into chr-by-chr mode and noecho mode
 *  method: use bits in termios
 */
{
	struct termios ttystate;

	tcgetattr( 0, &ttystate);		/* read curr. setting	*/
	ttystate.c_lflag	&= ~ICANON;	/* no buffering		*/
	ttystate.c_lflag	&= ~ECHO;	/* no echo either	*/
	ttystate.c_cc[VMIN] 	= 1;		/* get 1 char at a time */
	tcsetattr( 0, TCSANOW, &ttystate);	/* install settings	*/
}


int set_ticker( int n_msecs )
{
	struct itimerval new_timeset;
	long    n_sec, n_usecs;
	n_sec = n_msecs / 1000 ;		/* int part*/
	n_usecs = ( n_msecs % 1000 ) * 1000L ;  /* remainder*/

	new_timeset.it_interval.tv_sec = n_sec;		/* set reload */
	new_timeset.it_interval.tv_usec = n_usecs;  /* new ticker value */
	new_timeset.it_value.tv_sec     = n_sec;	/* store this */
	new_timeset.it_value.tv_usec    = n_usecs;	/* and this */

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}
