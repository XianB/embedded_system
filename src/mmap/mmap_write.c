/*
	遇到的问题:在原代码中的pmap和addr的声明是int类型而不是指针类型,所以导致了错误
 */

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define FILENAME "/tmp/test"
#define BUFLEN 256

typedef struct {
	char name[BUFLEN];
	int id;
}people;

int main(int argc, char ** argv)
{
	int i;
	unsigned int *pmap = 0;
	int fd;
	fd = open(FILENAME, O_CREAT | O_RDWR | O_TRUNC, 00777);
	assert(fd != -1);
	/*开始映射内存空间--文件*/
	pmap = (unsigned int *)mmap(0, sizeof(people), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	write(fd, "", sizeof(people));
	unsigned int *addr;
	addr = pmap;
	char tempname[30] = "xianb";
	int tempid = 253;
	/*寻址people中的id的位置*/
	addr = pmap + sizeof(char) * BUFLEN;
	/*拷贝people的两个属性的值*/
	memcpy((void *)pmap, tempname, strlen(tempname));
	memcpy((void *)addr, &tempid, sizeof(int));

	/*解除映射*/
	munmap((void *) pmap, sizeof(char) * BUFLEN);
	close(fd);
	printf("umap ok\r\n");
	return 0;
}
