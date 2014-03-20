#include "unpipc.h"

int main(int argc, char **argv)
{
	mqd_t		mqd;
	size_t		len;
	void		*ptr;	/* attention: not char* */
	unsigned int		prio;

	if (argc != 4) {
		fputs("usage: mq_send <name> <#byte> <priority>",
				stdout);
		exit(-1);
	}
	len = atoi(argv[2]);
	prio = atoi(argv[3]);

	if ( (mqd = mq_open(argv[1], O_WRONLY)) == -1) {
		perror("mq_open");
		exit(-1);
	}

	if ( (ptr = calloc(len, sizeof(char))) == NULL){
		perror("calloc");
		exit(-1);
	}
	strncpy((char*)ptr, "Hello", sizeof(ptr));

	if (mq_send(mqd, ptr, len, prio) == -1) {
		perror("mq_send");
		exit(-1);
	}

	exit(0);
}
