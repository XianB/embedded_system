﻿/***** testwrite.c *******/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
typedef struct
{
	char name[4];
	int age;
} people;
int main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	char temp;
	people *p_map;
	char* name = "/dev/shm/myshm2";
	key = ftok(name,0);
	if(key==-1)
	{
		perror("ftok error");
		return -1;
	}
	shm_id=shmget(key,4096,IPC_CREAT);	
	if(shm_id==-1)
	{
		perror("shmget error");
		return -1;
	}
	p_map=(people*)shmat(shm_id,NULL,0);
	temp='a';
	for(i = 0;i<10;i++)
	{
		temp+=1;
		memcpy((*(p_map+i)).name,&temp,1);
		(*(p_map+i)).age=20+i;
	}
	if(shmdt(p_map)==-1)
	{
		perror(" detach error ");
		return -1;
	}
	return 0;
}
