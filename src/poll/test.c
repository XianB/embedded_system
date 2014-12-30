#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd = 0;
	int fd_new = 0;
	char* writebuf = "I am kuangren";
	char* readbuf = malloc(sizeof("I am kuangren"));
	fd = open("/dev/char_dev",O_RDWR);
	fd_new = open("/dev/char_dev_new",O_RDWR);
	if (fd <= 0){
		printf("open failed.\n");
	} else {	
		int size=0;
		printf("fd = %d.\n", fd);
		printf("fd_new = %d.\n", fd_new);
		/*向/char_dev设备写入数据*/
		if((size=write(fd,writebuf,sizeof("I am KuangRen"))>0)){	
			printf("write success.\n%s\n",writebuf);
			size=0;
		}else {
			printf("write error.\n");
		}

		if((size=read(fd,readbuf,sizeof("I am KuangRen")))>0){
			printf("read success.\n%s\n",readbuf);
		} else {
			printf("read error.\n");
		}

		ioctl(fd, 0x01, NULL);
		ioctl(fd, 0x02, NULL);
		ioctl(fd, 0x03, NULL);

#if 1
		while(1)
		{
			struct pollfd fds[2];
			fds[0].fd = fd;
			fds[0].events = POLLIN | 1<<13;
			fds[1].fd = fd_new;
			fds[1].events = POLLIN | POLLOUT;

			if (poll(fds, 2, 300) > 0) 
			{
				if (fds[0].revents  ) {
					//do this event func();
					printf("POLL0 event = 0x%x\r\n",fds[0].revents);
				}
				if (fds[1].revents){
					printf("POLL1 in, events = 0x%x\r\n", fds[1].revents);
				}
			} else {
				printf("timeout\r\n");
			}
		}
	#endif
		close(fd);
	}

	printf("The End\n");
	return 0;
}
