#include "unpipc.h"

int main(int argc, char **argv)
{
	int			fd, writefd;
	size_t		len;
	ssize_t		n;
	char		buff[MAXLINE+1];

	if ((mkfifo("/tmp/fifo.1234", FILE_MODE) == -1)
			&& (errno != EEXIST)) {
		perror("mkfifo");
		exit(-1);
	}

	if ( (writefd = open("/tmp/fifo.1234", O_WRONLY, 0)) == -1) {
		perror("open");
		exit(-1);
	}

	fgets(buff, MAXLINE, stdin);
	len = strlen(buff);
	if (buff[len-1] == '\n') {
		buff[len-1] = '\0';
		len--;
	}

	if ( (fd = open(buff, O_RDONLY)) == -1) {
		snprintf(buff+len, sizeof(buff)-len, ":can't open, %s\n", 
				strerror(errno));	
		n = strlen(buff);
		write(writefd, buff, n);
	} else {
		while( (n = read(fd, buff, MAXLINE)) > 0)	
			write(writefd, buff, n);
		close(fd);
	}

	close(writefd);
	exit(0);
}
