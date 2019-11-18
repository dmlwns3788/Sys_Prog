#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include <sys/wait.h>
#define oops(m,x)	{ perror(m); exit(x); }

void be_dc(int in[2], int out[2]);
void be_bc(int todc[2], int fromdc[2]);
void fatal(char mess[]);

int main()
{
	int	pid, todc[2], fromdc[2];	/*equipment	*/

    //todo
	//make two pipes
	if(pipe(todc) == -1 || pipe(fromdc) == -1)
		oops("pipe failed", 1);

	// test
	printf("todc[0]  : %d, todc[1]  : %d \n", todc[0], todc[1]);
	printf("fromdc[0]: %d, fromdc[1]: %d\n", fromdc[0], fromdc[1]);

	/* get a process for user interface */
    //todo
	if((pid=fork()) == -1) {
		oops("cannot fork", 2);
	}
	else {
		be_bc(todc, fromdc);
		wait(NULL);
	}
}

void be_dc(int in[2], int out[2])
{
    //todo
	if(dup2(in[0], 0) == -1)
		oops("dc: cannot redirect stdin", 3);
	close(in[0]);
	close(in[1]);

	//setup stdout to pipeout
	if(dup2(out[1], 1) == -1)
		oops("dc: cannot redirect stdout", 4);
	close(out[1]);
	close(out[0]);

	//now execl dc with the -option
	execlp("dc", "dc", "-", NULL);
	oops("cannot run dc", 5);
}


void be_bc( int todc[2], int fromdc[2])
{
	int	num1, num2;
	char	operation[BUFSIZ], message[BUFSIZ], *fgets();
	FILE	*fpout, *fpin, *fdopen();

	/* setup */
    //todo
	close(todc[0]);
	close(fromdc[1]);

	fpout = fdopen(todc[1], "w");
	fpin = fdopen(fromdc[0], "r");

	if( fpout == NULL || fpin == NULL )
		fatal("Error converting pipes to streams");

	/* main loop */
    //todo
	while(printf("tinybc: "), fgets(message, BUFSIZ, stdin) != NULL)
	{
		if(sscanf(message, "%d[-+*/^]%d", &num1, operation, &num2) != 3)
		{
			printf("syntax error\n");
			continue;
		}

		if(fprintf(fpout, "%d\n%d\n%cp\n", num1, num2, *operation) == EOF)
			fatal("Error writing");
		fflush(fpout);

		if(fgets(message, BUFSIZ, fpin) == NULL)
			break;
		printf("%d %c %d = %s", num1, *operation, num2, message);
	}
	fclose(fpout);
	fclose(fpin);
}

void fatal( char mess[] )
{
	fprintf(stderr, "Error: %s\n", mess);
	exit(1);
}


