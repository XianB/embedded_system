#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO_FILE "/tmp/myfifo"
int main(void)
{
	char buf[100];
	int n = 0; 
	int fd;

	if ((mkfifo(FIFO_FILE, S_IRWXU < 0) && (errno != EEXIST))) {
		perror("mkfifo error");
		exit(-1);
	}

	if ((fd = open(FIFO_FILE, O_RDONLY| O_NONBLOCK)) < 0) {
		perror("open error");
		exit(-1);
	}

	while (1) {
		if ((n = read(fd, buf, 100)) < 0) {
			/*已经被只写打开:返回 EAGAIN 错误*/
			if (errno == EAGAIN) {
				printf("No data yet\n");
			}
		} else if (n == 0)
			printf("No opened by write_only\n");
		else if (n > 0)
			write(STDOUT_FILENO, buf, n);
			sleep(1);
	}

	unlink(FIFO_FILE);
	return 0;
}
