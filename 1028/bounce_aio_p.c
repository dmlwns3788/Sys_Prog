#include	<unistd.h>
#include	<stdio.h>
#include	<curses.h>
#include	<signal.h>
#include	<aio.h>
#include 	<string.h>
#include	<termios.h>

/* The state of the game 			*/
#define	MESSAGE	"hello"
#define	BLANK	"     "

int	row   = 10;	/* current row		*/
int	col   =  0;	/* current column	*/
int	dir   =  1;	/* where we are going	*/
int	delay = 200;	/* how long to wait	*/
int	done  = 0;
struct aiocb kbcbuf;	/* an aio control buf   */

void set_cr_noecho_mode(void);
int set_ticker(int);

void main(void)
{
	void	on_alarm(int);	/* handler for alarm	*/
	void	on_input(int);	/* handler for keybd    */
	void	setup_aio_buffer();
	initscr();		/* set up screen */

	set_cr_noecho_mode(); //gsjung

	clear();
	signal(SIGIO, on_input);          /* install a handler        */
	setup_aio_buffer();	  	  /* initialize aio ctrl buff */
	
	//todo
    //
    signal(SIGALRM, on_alarm);        /* install alarm handler    */
	set_ticker(delay);		  /* start ticking     */
	mvaddstr( row, col, MESSAGE );    /* draw initial image       */
	while( !done )			  /* the main loop */
		pause();
	endwin();
}

 /*
 *  handler called when aio_read() has stuff to read
 *  First check for any error codes, and if ok, then get the return code
 */
void on_input(int snum)
{	
	int  c;
	char *cp = (char *) kbcbuf.aio_buf;	/* cast to char * */
	/* check for errors */
	if ( aio_error(&kbcbuf) != 0 )
		perror("reading failed");
	else 
		/* get number of chars read */
		//todo
        {
			c = *cp;
			if ( c == 'Q' || c == EOF )
				done = 1;
			else if ( c == ' ' )
				dir = -dir;
		}

	/* place a new request */
    //todo
    //
}

void on_alarm(int snum)
{
	signal(SIGALRM, on_alarm);	/* reset, just in case	*/
	mvaddstr( row, col, BLANK );	/* clear old string	*/
	col += dir;			/* move to new column	*/
	mvaddstr( row, col, MESSAGE );	/* draw new string	*/
	refresh();			/* and show it		*/
	/*
	 * now handle borders
	 */
	if ( dir == -1 && col <= 0 )
		dir = 1;
	else if ( dir == 1 && col+strlen(MESSAGE) >= COLS )
		dir = -1;
}

 /*
 * set members of struct.  
 *   First specify args like those for read(fd, buf, num)  and offset
 *   Then  specify what to do (send signal) and what signal (SIGIO)
 */
void setup_aio_buffer()
{
	static char input[1];		      /* 1 char of input */

	/* describe what to read */
    //todo

	/* describe what to do when read is ready */
	kbcbuf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
	kbcbuf.aio_sigevent.sigev_signo  = SIGIO;  /* send sIGIO   */
}

void set_cr_noecho_mode(void)
{
	struct termios ttystate;
	
	tcgetattr( 0, &ttystate);
	ttystate.c_lflag	&= ~ICANON;
	ttystate.c_lflag	&= ~ECHO;
	ttystate.c_cc[VMIN] 	= 1;
	tcsetattr( 1, TCSANOW, &ttystate);
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
