#include "unpipc.h"

/*  man 7 mq_overview(The Linux Part)
 *  $mkdir /dev/mqueue
 *	$mount -t mqueue none /dev/mqueue
 */

int main(int argc, char **argv)
{
	int		flags;
	mqd_t	mqd;	
	pid_t	pid;
	char	buff[MAXLINE];

	flags = O_RDWR | O_CREAT;
	
	if ( (pid = getpid()) == -1) {
		perror("getpid");
		exit(-1);
	}
	snprintf(buff, sizeof(buff), "/mq.%ld", (long)pid);

	if ( (mqd = mq_open(buff, flags, FILE_MODE, NULL)) == -1) {
		perror("mq_open");
		exit(-1);
	}

	if (mq_close(mqd) == -1) {
		perror("mq_close");
		exit(-1);
	}

		/* destroy the mq */
	/*	if(mq_unlink(buff) == -1) {
		perror("mq_unlink");
		exit(-1);
	}	*/
	exit(0);
}
