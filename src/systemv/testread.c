#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

typedef struct {
	char name[4];
	int age;
}people;

int main(int argc, char ** argv)
{
	int shm_id, i;
	key_t key;
	people * p_map = NULL;
	char * name = "/dev/shm/myshm2";
//	char * name = "/dev/shm";
	key = ftok(name, 0);
	if (key == -1) {
		perror("ftok error");
		return -1;
	}

	shm_id = shmget(key, 4096, IPC_CREAT | 0666);
	if (shm_id == -1) {
		perror("shmget error");
		return -1;
	}

	p_map = (people*)shmat(shm_id, NULL, 0);
	if ( p_map == NULL) {
		printf("shmat failed\r\n");
		return -1;
	}

	for (i = 0; i < 10; i++) {
		printf("name: %s \n", (*(p_map + i)).name);
		printf("age %d\n", (*(p_map + i)).age);
	}
	if(shmdt(p_map) == -1) {
		perror("detach error");
	}
	return 0;
}
