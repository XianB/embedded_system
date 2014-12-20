/**************
 * pipetest_prog.c *

 **************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

int main(unsigned int argc, unsigned char **argv)
{
	int pipe_fd[2];
	pid_t pid;
	char r_buf[100];
	char w_buf[4];
	int r_num;
	int cmd;

	memset(r_buf,0,sizeof(r_buf));
	memset(w_buf,0,sizeof(r_buf));

	if(pipe(pipe_fd)<0)
	{
		printf("FILE: %s, LINE: %d.pipe create error\n",__FILE__, __LINE__);
		return -1;
	}

	if((pid=fork())==0)	//子进程中
	{
		printf("##########################\n");
		close(pipe_fd[1]);	//关闭写端
		sleep(3);	//确保进程关闭写端
		r_num=read(pipe_fd[0],r_buf,100);
		printf("child read num is %d , the data read from the pipe is %d\n",r_num,atoi(r_buf));
		close(pipe_fd[0]);	//关闭读端
		//exit(0);
	}
	else if(pid>0)	//父进程中
	{
		close(pipe_fd[0]);	//关闭读端
		strcpy(w_buf,"111");
		if(write(pipe_fd[1],w_buf,4)!=-1)
		{
			printf("parent write over\n");
		}
		close(pipe_fd[1]);	//关闭写端
		printf("parent close fd[1]-write over\n");
		sleep(3);
	}	
	return 0;
}
