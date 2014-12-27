#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(int argc, char ** argv)
{
	int pipe_fd[2];
	pid_t pid;
	char r_buf[100];
	char w_buf[4];
	int r_num;
	int cmd;

	memset(r_buf, 0, sizeof(r_buf));
	memset(w_buf, 0, sizeof(w_buf));

	if (pipe(pipe_fd) < 0) {
		printf("FILE: %s, LINE: %d.pipe create error\n", __FILE__, __LINE__);
		return -1;
	}

	/*子进程关闭写端口,父进程关闭读端口,建立一个从父进程到到子进程的通道*/
	if ((pid = fork()) == 0) {
		printf("##################\n");
		close(pipe_fd[1]);		/*子进程关闭写端口*/
		/*等待父进程向端口写入数据,然后子进程才能读数据*/
		sleep(3);
		/*父进程写入数据后进行读写数据*/
		r_num = read(pipe_fd[0], r_buf, 100);
		printf("child read num is %d, the data read from the pipe is %d\n", r_num, atoi(r_buf));
		close(pipe_fd[0]);
	} else if (pid > 0) {
		close(pipe_fd[0]);		/*父进程关闭读端口*/
		strcpy(w_buf, "111");
		/*向pipe里写入数据*/
		if (write(pipe_fd[1], w_buf, 4) != -1) {
			printf("parent write over\n");
		}
		/*父进程关闭写端口,也就是关闭了pipe了*/
		close(pipe_fd[1]);
		printf("parent close fd[1]-write over\n");
		sleep(3);
	}
	return 0;
}
