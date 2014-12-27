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
	pmap = (unsigned int *)mmap(0, sizeof(people), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	write(fd, "", sizeof(people));
	unsigned int *addr;
	addr = pmap;
	char tempname[30] = "xianb";
	int tempid = 253;
	addr = pmap + sizeof(char) * BUFLEN;
	memcpy((void *)pmap, tempname, strlen(tempname));
	memcpy((void *)addr, &tempid, sizeof(int));

	munmap((void *) pmap, sizeof(char) * BUFLEN);
	close(fd);
	printf("umap ok\r\n");
	return 0;
}
