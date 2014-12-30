#include <stdio.h>  
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
#include <sys/time.h>   
#include <signal.h>   
#include <sys/socket.h>   
#include <sys/un.h>   

#define UNIX_DOMAIN "/tmp/UNIX2.domain"   

static char recv_php_buf[256];  //接收client数据的缓冲  
static int recv_php_num=0;      //接收client数据的总长度  

const char recv_php_buf1[20]={0x06,0x05,0x04,0x03,0x02,0x01,0x00};  

int main(int argc, char **argv)  
{  
	socklen_t clt_addr_len;   //长度
	int listen_fd;   
	int com_fd;   
	int ret=0;   
	int i;   

	int len;   
	struct sockaddr_un clt_addr;   /*保存着接受的客户端的地址*/

	struct sockaddr_un srv_addr;   /*srv_addr是服务器*/

	while(1)  
	{  
		/*创建用于通信的套接字，通信域为UNIX通信域   */
		/*listen_fd是套接字*/

		listen_fd=socket(AF_UNIX,SOCK_STREAM,0);   

		if(listen_fd<0)  
		{   
			perror("cannot create listening socket");   
			continue;   
		}   
		else  
		{  
			while(1)  
			{  
				/*设置服务器地址参数   */
				srv_addr.sun_family=AF_UNIX;   
				/*设置服务器的路径 */
				strncpy(srv_addr.sun_path,UNIX_DOMAIN,sizeof(srv_addr.sun_path)-1);   
				unlink(UNIX_DOMAIN);   
				/*绑定套接字与服务器地址信息   */
				ret=bind(listen_fd,(struct sockaddr*)&srv_addr,sizeof(srv_addr));   
				if(ret==-1)  
				{   
					perror("cannot bind server socket");   
					close(listen_fd);   
					unlink(UNIX_DOMAIN);   
					break;   
				}   
				//对套接字进行监听，判断是否有连接请求   
				ret=listen(listen_fd,1);   
				if(ret==-1)  
				{   
					perror("cannot listen the client connect request");   
					close(listen_fd);   
					unlink(UNIX_DOMAIN);   
					break;   
				}   
				chmod(UNIX_DOMAIN,00777);//设置通信文件权限  
				while(1)  
				{  
					/*当有连接请求时，调用accept函数建立服务器与客户机之间的连接   */
					len=sizeof(clt_addr);   
//					printf("1\n");
					/*accept()是阻塞等待的
					 clt_addr保存着接受的客户端的地址
					 */
					com_fd=accept(listen_fd,(struct sockaddr*)&clt_addr,&len);   
//					printf("2\n");
					if(com_fd<0)  
					{   
						perror("cannot accept client connect request");   
						close(listen_fd);   
						unlink(UNIX_DOMAIN);   
						break;   
					}   
					//读取并输出客户端发送过来的连接信息   
					memset(recv_php_buf,0,256);   
					/*read from socket com_fd , and save in recv_php_buf*/

					recv_php_num=read(com_fd,recv_php_buf,sizeof(recv_php_buf));   
					printf("\n=====recv=====\n");  
					for(i=0;i<recv_php_num;i++)
					{	
						printf("%d ",recv_php_buf[i]);   
					}
					printf("\n");  

					/*write from recv_php_buf send to socket com_fd the client*/
					write(com_fd,recv_php_buf1,recv_php_num);  
					printf("\n=====send=====\n");  
					for(i=0;i<recv_php_num;i++)
					{	
						printf("%d ",recv_php_buf1[i]);   
					}
					printf("\n");  
					//write(com_fd,recv_php_buf,20);  
					close(com_fd);//注意要关闭连接符号，不然会超过连接数而报错  
				}  

			}  

		}  
	} 
	return 0;
}	
