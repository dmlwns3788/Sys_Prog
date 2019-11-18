/* pipe.c
 *	* Demonstrates how to create a pipeline from one process to another
 *	* Takes two args, each a command, and connects
 *	  av[1]'s output to input of av[2]
 *	* usae: pipe command1 command2
 *	  effect: command1 | command2
 *	* Limitations: commands do not take arguments
 *	* uses execlp() since known number of args
 *	* Note: exchange child and parent and watch fun
 */

#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>

#define 	oops(m,x)	{ perror(m); exit(x); }

main(int ac, char **av)
{
	int	thepipe[2],		/* two file descriptors */
		newfd,			/* useful for pipes	*/
		pid;			/* and the pid		*/

	if ( ac !=3 ) {
		fprintf( stderr, "usage : pipe cmd1 cmd2\n" );
		exit(1);
	}
    //todo
	if(pipe(thepipe) == -1)
		oops("Cannot get a pipe", 1);

	if((pid = fork()) == -1)
		oops("Cannot fork", 2);

	if(pid > 0) {
		close(thepipe[1]);

		if(dup2(thepipe[0], 0) == -1)
			oops("Could not redirect stdin", 3);

		close(thepipe[0]);
		execlp(av[2], av[2], NULL);
		oops(av[2], 4);
	}

	close(thepipe[0]);
	if(dup2(thepipe[1], 1) == -1)
		oops("could not redirect stdout", 4);

	close(thepipe[1]);
	execlp(av[1], av[1], NULL);
	oops(av[1], 5);

}
