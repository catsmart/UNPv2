#include "unpipc.h"

int main(int argc, char **argv)
{
	mqd_t			mqd;
	void			*buff;
	size_t			len;
	ssize_t			n;
	unsigned int	prio;
	struct mq_attr	attr;

	if (argc != 2) {
		fputs("usage: mq_receive <name>\n", stdout);
		exit(-1);
	}

	if ( (mqd = mq_open(argv[1], O_RDONLY)) == -1) {
		perror("mq_open");
		exit(-1);
	}

	if(mq_getattr(mqd, &attr) == -1) {
		perror("mq_getattr");
		exit(-1);
	}

	if ( (buff = malloc(attr.mq_msgsize)) == NULL) {
		perror("malloc");
		exit(-1);
	}

	if ( (n = mq_receive(mqd, buff, attr.mq_msgsize, &prio)) == -1) {
		perror("mq_receive");
		exit(-1);
	}

	printf("read %ld bytes, priority = %u\n", (long)n, prio);
	printf("%s\n", (char*)buff);
	exit(0);
}
