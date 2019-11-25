            /* timeserv.c - a socket-based time of day server
 */
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define PORTNUM 13000 
#define HOSTLEN 256
#define oops(msg)	{ perror(msg); exit(1); }

int main(int ac, char *av[])
{
	struct sockaddr_in saddr;	/* build our address here	*/
	struct hostent *hp;		/* this part of our		*/
	char hostname[HOSTLEN];		/* address			*/
	int sock_id, sock_fd;		/* line id, file descriptor	*/
	FILE *sock_fp;			/* use socket as steam		*/
	char *ctime();			/* convert secs to string	*/
	time_t thetime;			/* the time we report		*/
    //todo
	sock_id = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_id == -1)
		oops("socket");
	bzero((void*)& saddr, sizeof(saddr));
	hp = gethostbyname(hostname);
	bcopy((void*)hp->h_addr, (void*)& saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;

	if (bind(sock_id, (struct sockaddr*) &saddr, sizeof(saddr)) != 0)
		oops("bind");

	if (listen(sock_id, 1) != 0)
		oops("listen");
	while (1) {
		sock_fd = accept(sock_id, NULL, NULL);
		printf("got a call !\n");
		if (sock_fd == -1)
			oops("accept");
		sock_fp = fdopen(sock_fd, "w");
		if (sock_fp == NULL)
			oops("fdopen");
		thetime = time(NULL);
		fprintf(sock_fp, "The time here is ..");
		fprintf(sock_fp, "%s", ctime(&thetime));
		fclose(sock_fp);
	}
}
