#include "unpipc.h"

void server(int, int), client(int, int);

int main(int argc, char **argv)
{
	int		pipe1[2], pipe2[2];		/* learn this declare style */
	pid_t	childpid;

	if(pipe(pipe1) == -1) {
		perror("pipe");
		exit(-1);
	}
	if(pipe(pipe2) == -1) {
		perror("pipe");
		exit(-1);
	}

	if( (childpid = fork()) == 0) {		/* child */
		close(pipe1[1]);
		close(pipe2[0]);

		server(pipe1[0], pipe2[1]);
		exit(0);
	}
		/* parent */
	close(pipe1[0]);
	close(pipe2[1]);

	client(pipe2[0], pipe1[1]);

	waitpid(childpid, NULL, 0);		/* wait for child to terminate */
	exit(0);
}

void client(int readfd, int writefd)
{
	ssize_t		n;
	size_t		len;
	char		buff[MAXLINE];
	
		/* read pathname */
	fgets(buff, MAXLINE, stdin);
	len = strlen(buff);
	if(buff[len-1] == '\n')
		len--;		/* delete newline from fgets() */

		/* write pathname to IPC channel */
	if(write(writefd, buff, len) == -1) {
		perror("write");
		exit(-1);
	}

		/* read from IPC, write to standard output */
	while( (n = read(readfd, buff, MAXLINE)) >0)
		write(STDERR_FILENO, buff, n);
}


void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE+1];		/* +1 in order to terminate the pathname */

		/* read pathname from IPC */
	if( (n = read(readfd, buff, MAXLINE)) == 0) { /* block here */
		perror("read");
		exit(-1);
	}
	buff[n] = '\0';

	printf("buff:[%s]\n", buff);

	if( (fd = open(buff, O_RDONLY)) < 0) {
		snprintf(buff + n, sizeof(buff) - n, ":can't open, %s\n", 
				strerror(errno));
		n = strlen(buff);
		write(writefd, buff, n);
	} else {
			/* open succeeded: copy file to IPC channel*/
		while( (n = read(fd, buff, MAXLINE)) > 0)
			write(writefd, buff, n);
		close(fd);
	}	
}
