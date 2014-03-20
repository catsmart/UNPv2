#include "unpipc.h"

int main(int argc, char **argv)
{
	int readfd;
	ssize_t n;
	char buff[MAXLINE];

	if((mkfifo("/tmp/fifo.1234", FILE_MODE) == -1) 
			&& (errno != EEXIST)){
		perror("mkfifo");
		exit(-1);
	}

	if( (readfd = open("/tmp/fifo.1234", O_RDONLY, 0)) == -1) {
		perror("open");
		exit(-1);
	}
	
	for(;;) {
		while( (n = read(readfd, buff, MAXLINE)) > 0)
			write(STDOUT_FILENO, buff, n);
	}

	close(readfd);
	unlink("/tmp/fifo.1234");
	exit(0);
}
