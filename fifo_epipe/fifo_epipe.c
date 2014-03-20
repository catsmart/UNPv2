#include "unpipc.h"

int main(int argc, char **argv)
{
	int			writefd;
	size_t		n;
	char		buff[MAXLINE+1];

	if ( (mkfifo("/tmp/fifo.1234", FILE_MODE) == -1) 
			&& (errno != EEXIST)) {
		perror("mkfifo");
		exit(-1);
	}

		/* O_NONBLOCK: opening for write only will fail whit ENXIO cause no reader. */
	if ( (writefd = open("/tmp/fifo.1234", O_WRONLY | O_NONBLOCK, 0)) == -1) {
		perror("open");
		exit(-1);
	}

	fgets(buff, MAXLINE, stdin);
	n = strlen(buff);
	if (buff[n-1] == '\n')
		n--;		/* delete newline from fgets() */

	write(writefd, buff, n);

	close(writefd);
	exit(0);
}
