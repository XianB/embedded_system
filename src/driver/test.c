#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <unistd.h>
int main(int argc, char **argv)
{
	unsigned int writenum =0x253 ;
	unsigned int readnum=0;
	int ret = 0;
	int fd = open("/dev/globalvar", O_RDWR, S_IRUSR|S_IWUSR);
	assert(fd != 0);
#if 1
	if (write(fd, &writenum, sizeof(writenum)) < 0 )
	{
		printf("write failed.\r\n");
		close(fd);
	}
#endif
	if ( (ret = read(fd, &readnum, sizeof(readnum))) < 0 )
	{
		printf("read failed.\r\n");
		close(fd);
	}
	printf("readnum var is 0x%x\r\n",readnum);
#if 0
	ioctl(fd, 0x11, NULL);
#endif
	close(fd);
	return 0;
}
