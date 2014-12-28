#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	char name[4];
	int age;
}people;

int main(int argc, char ** argv)
{
	int shm_id, i;
	key_t key;
	char temp;
	people * p_map;
	char * name = "/dev/shm/myshm2";
//	char * name = "/dev/shm";
	/*获得键值*/
	key = ftok(name, 0);
	if (key == -1) {
		perror("ftok error");
		return -1;
	}

	/*用来获得共享内存区域的 ID,如果不存在指定的共享区域就创建相应的区域。*/
	shm_id = shmget(key, 4096, IPC_CREAT | 0666);
	if (shm_id == -1) {
		perror("shmget error");
		return -1;
	}

	/*把共享内存区域映射到调用 进程的地址空间 中去,这样,进程就可以方便地对共享区域进行访问操作。*/
	/*现在可以像访问进程内部的内存空间一样使用共享内存区域的内存了*/
	p_map = (people*)shmat(shm_id, NULL, 0);

	temp = 'a';

	for (i = 0; i < 10; i++) {
		temp += 1;
		memcpy((*(p_map + i)).name, &temp, 1);
		(*(p_map + i)).age = 20 + i;
	}

	/*调用用来解除进程对共享内存区域的映射。*/

	if (shmdt(p_map) == -1) {
		perror(" detach error");
		return -1;
	}

	return 0;
}
