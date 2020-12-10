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
#include <stdlib.h>
#include <string.h>
#define SIZE 2048

struct line{
	char string[SIZE];
};

int writer(int from, int to, int shm_id, int sem_id);
void wait_nil(int id, int sem_ind1);
void minus(int id, int sem_ind1);
void plus(int id, int sem_ind1);

int main(int argc, char *argv[])
{
	char *path = "/home/user/semestr3/sem8/server.c";
	key_t key = ftok(path, 1);
	int sem_id = semget(key, 2, 0666); // crate  посмотреть и удалить
	int shm_id = shmget(key, sizeof(struct line), 0666);


	if (argc == 1)
	{
		int incycle = 1;
		while (incycle)
		{
			int real_size = writer(0, 1, shm_id, sem_id);
			if (real_size != 0)
			{
				incycle = 1;
			}
			else
			{
				incycle = 0;
			}
		}

	}
	else
	{	int i = 1;
		for (i; i < argc; i++)
		{
			writer(open(argv[i], O_RDONLY), 1, shm_id, sem_id);	
		}
		
	}
	return 0;
}

int writer(int from, int to, int shm_id, int sem_id)
{
	if (from < 0)
	{
		perror("client");
		return 0;
	}
	else
	{
		FILE *stream = fdopen(from, "r");
		if (stream == NULL)
		{
			perror("client");
			return 0;
		}
		
		void *shared_memory = (void *)0;
		struct line *shared_stuff;
		shared_memory = shmat(shm_id, (void *)0, 0);
		shared_stuff = (struct line *)shared_memory;
		char *res_fgets = (void *)1;
		char buff[SIZE];


		while (res_fgets)
		{
			wait_nil(sem_id, 1);
			res_fgets = fgets(buff, SIZE, stream);
			if (res_fgets)
			{
				strncpy(shared_stuff->string, buff, SIZE);
			}
			plus(sem_id, 1);
			minus(sem_id, 0);
		}
	}
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