#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(int argc, char **argv)
{
	int pipe_fd[2];
	pid_t pid;
	char r_buf[4];
	char* w_buf;
	int writenum;
	int cmd;
	
	memset(r_buf,0,sizeof(r_buf));
	if(pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	
	if((pid=fork())==0)
	{
		/*子进程关闭写端口,这样的话pipe就不可用了*/
//		close(pipe_fd[0]);
		close(pipe_fd[1]);
		sleep(2);	
		exit(0);
	}
	else if(pid>0)
	{
		sleep(1);  //等待子进程完成关闭读端的操作
		/*父进程关闭读端口*/
		close(pipe_fd[0]);
		w_buf="111";
		printf("FILE: %s, LINE: %d\r\n", __FILE__, __LINE__);
		/*父进程向子进程写入数据*/
		writenum=write(pipe_fd[1],w_buf,4)==-1;

		if((writenum=write(pipe_fd[1],w_buf,4))==-1)
			printf("write to pipe error\n");
		else	
			printf("the bytes write to pipe is %d \n", writenum);
		
		printf("FILE: %s, LINE: %d\r\n", __FILE__, __LINE__);
		close(pipe_fd[1]);
	}	
	return 0;
}
