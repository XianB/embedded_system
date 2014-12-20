﻿#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <malloc.h>  
#include <sys/types.h>  
#include <errno.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <sys/select.h>  
#include <unistd.h>  
#include <termios.h>  
#include <sys/stat.h>  
/**********定时器头文件***************/  
#include <sys/time.h>   
#include <signal.h>   
/***********进程间SOCKET通信头文件**********/  
#include <sys/socket.h>   
#include <sys/un.h>   

#include <sys/ioctl.h>  
#pragma pack(1)         //设定为1字节对齐  
#define UNIX_DOMAIN2 "/tmp/UNIX2.domain"   
static char recv_php_buf[256]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};      
struct test  
{  
	char a;  
	int b;  
	int c;  
}se;  

int  main(int argc, char **argv)  
{  
	int connect_fd;  
	int ret=0;  
	int i;  
	static struct sockaddr_un srv_addr;   
	printf("IPC通信线程\n");  
	//while(1)  
	//{  
	//创建用于通信的套接字，通信域为UNIX通信域   
	connect_fd=socket(AF_UNIX,SOCK_STREAM,0);   
	printf("%d\n",connect_fd);   
	if(connect_fd<0)  
	{   
		perror("cannot create communication socket");  
		printf("%d\n",connect_fd);   
		return -1;
	}   
	else  
	{  
		//while(1)  
		//{  
		srv_addr.sun_family=AF_UNIX;   
		strcpy(srv_addr.sun_path,UNIX_DOMAIN2);  

		//连接服务器   
		ret=connect(connect_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));   
		if(ret==-1)  
		{   
			close(connect_fd);   
			printf("connect fail\n");  
			//break;            //重新创建socket  
			close(connect_fd);
			return -1;
		}  
		else  
		{     
			//否则，连接服务器成功  
			//while(1)  
			//{  
			se.a=0x01;  
			se.b=0x01020304;  
			se.c=0x05060708;  
			write(connect_fd,recv_php_buf,20);//将数据传送到外部应用程序,发送实际长度  
			//write(connect_fd,&se,sizeof(struct test));  
			memset(recv_php_buf,0,sizeof(recv_php_buf));                             //清空socket_buf  
			//sleep(1);  
			//fcntl(connect_fd,F_SETEL,O_NONBLOCK);  
			read(connect_fd,recv_php_buf,sizeof(recv_php_buf));  
			printf("receive from server over\n");  
			for(i=0;i<20;i++)  
			{
				printf("%x ",recv_php_buf[i]);  
			}
			//printf("%x ",se.c);  
			printf("\n");
			close(connect_fd);  
			//}  
		}  

		//}  
		//close(connect_fd);  
	}  

	//}  
	return 0; 
}

