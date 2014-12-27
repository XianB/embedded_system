#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO_FILE "/tmp/myfifo"
int main(void)
{
	int fd = 0;
	int n;
	char buf[100];

	if ((fd = open(FIFO_FILE, O_WRONLY | O_NONBLOCK)) < 0) {
		perror("open error");
		exit(-1);
	}
	while (1) {
		fgets(buf, 100, stdin);
		n = strlen(buf);
		if ((n = write(fd, buf, n)) < 0) {
			if (errno == EAGAIN) {
				printf("The FIFO has not been read yet, Please try later\n");
			}
		}
	}
	return 0;
}
