/*-------------map_normalfile1.c-----------*/
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
//#define FILENAME "/home/zhangxiao/embededSystem/example/Mmap/test"
#define FILENAME "/tmp/test"
#define BUFLEN	256
typedef struct
{
	char name[BUFLEN];
	int  id;
}people;

int main(int argc, char** argv) // map a normal file as shared mem:
{
	int i;
	unsigned int pmap=0;
	int fd;
	fd=open(FILENAME ,O_CREAT|O_RDWR,00777 );
	assert(fd !=-1);
	pmap = (unsigned int)mmap(0,sizeof(people),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	unsigned int addr;
	addr=pmap;
	addr = pmap + sizeof(char)*BUFLEN;	
	printf("id=%d   name=%s\n\r",*((int *)addr),(char *)pmap);	
	munmap((void *) pmap,sizeof(char)*BUFLEN);
	close(fd);
	printf("umap ok\r\n");
	return 0;
}

