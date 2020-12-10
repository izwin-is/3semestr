#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define SIZE 2048

struct line{
	char string[SIZE];
};

void wait_nil(int id, int sem_ind1);
void minus(int id, int sem_ind1);
void plus(int id, int sem_ind1);

int main(int argc, char const *argv[])
{
	char *path = "server.c";
	key_t key = ftok(path, 1);
	int sem_id = semget(key, 2, (IPC_CREAT | 0666));
	int shm_id = shmget(key, sizeof(struct line), (IPC_CREAT | 0666));

	semctl(sem_id, 0, SETVAL, 1); // wait writting
	semctl(sem_id, 1, SETVAL, 0); // wait sharing


	struct line *shared_stuff;
	shared_stuff = (struct line *)shmat(shm_id, (void *)0, 0);

	while (1)
	{
		wait_nil(sem_id, 0);
		printf("%s", shared_stuff->string);

		plus(sem_id, 0);
		minus(sem_id, 1);
	}

	return 0;
}


void wait_nil(int id, int sem_ind1)
{
	struct sembuf s1 = {sem_ind1, 0, 0};
	struct sembuf arr[1] = {s1};
	semop(id, arr, 1);
}

void minus(int id, int sem_ind1)
{
	struct sembuf s1 = {sem_ind1, -1, 0};
	struct sembuf arr[1] = {s1};
	semop(id, arr, 1);
}

void plus(int id, int sem_ind1)
{
	struct sembuf s1 = {sem_ind1, 1, 0};
	struct sembuf arr[1] = {s1};
	semop(id, arr, 1);
}