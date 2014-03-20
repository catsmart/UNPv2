#include "unpipc.h"

/*  man 7 mq_overview(The Linux Part)
 *  $mkdir /dev/mqueue
 *	$mount -t mqueue none /dev/mqueue
 */

int main(int argc, char **argv)
{
	mqd_t			mqd;
	struct mq_attr	attr;
	char			buff[MAXLINE];
	
	if (argc != 2) {
		fputs("usage: mq_attr <name>\n", stdout);
		exit(-1);
	}
			/* a bug: if ( (mqd = mq_open(argv[1], O_RDONLY) == -1)) { 
			 * see what will happen. 
			 * the program first compare mq_open() and  -1 
			 * then return -1, and mqd was set -1 
			 * so mq_getattr return "bad file descriptor" */

	if ( (mqd = mq_open(argv[1], O_RDONLY)) == -1) {
		perror("mq_open");
		exit(-1);
	}

	if (mq_getattr(mqd, &attr) == -1) {
		perror("mq_getattr");
		exit(-1);
	}

	snprintf(buff, sizeof(buff), 
			"mq_flags:%ld mq_maxmsg:%ld mq_msgsize:%ld mq_curmsgs:%ld\n", 
			attr.mq_flags, attr.mq_maxmsg, 
			attr.mq_msgsize, attr.mq_curmsgs);
	fputs(buff, stdout);

	mq_close(mqd);
	exit(0);
}
