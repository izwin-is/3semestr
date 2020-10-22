#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

void captain(int id);
void person(int id, int i, int n);
void wait_nil(int id, int sem_ind);
void minus(int id, int sem_ind, int op);
void plus(int id, int sem_ind, int op);


int main(int argc, char const *argv[])
{
	setbuf(stdout, NULL);
	int n = atoi(argv[1]), k = atoi(argv[2]), m = atoi(argv[3]);
	int id = semget(IPC_PRIVATE, 4, (IPC_CREAT | IPC_EXCL | 0700));
	int i = 1;
	semctl(id, 0, SETVAL, k); // ship
	semctl(id, 1, SETVAL, m); // ludder
	semctl(id, 2, SETVAL, 1); // ludder down
	semctl(id, 3, SETVAL, 1); // ship is not moving

	// struct sembuff to_ludder = {1, -1};
	// struct sembuff from_ludder = {1, 1};
	// struct sembuff to_ship = {0, -1};
	// struct sembuff from_ship = {0, 1};
	// struct sembuff ludder_ud;
	// struct sembuff ludder_down;

	pid_t pid;
	pid = fork();
	if (pid == 0) // Child
	{
		captain(id);
		return 0;
	}
	for (i; i < n + 1; ++i)
	{
		pid = fork();
		if (pid == 0)
		{
			printf("%d\n", i);
			person(i, id, n);
			return 0;
		}
	}

	int stat;
	while (wait(&stat) != -1);
	semctl(id, IPC_RMID, NULL);
	return 0;
}
void captain(int id)
{
	printf("Cap: Approaching to the port\n");
	minus(id, 3, 1); // Устанавливаем в 0 семофор, отвечающий за движение корабля
	printf("Cap: The ship has stopped\n");
	minus(id, 2, 1); //  Опускаем трап 
	printf("Cap: The ludder is down\n");
	wait_nil(id, 0); //  Ждем заходящих людей
	printf("Cap: SUPER TRAVEL HAS STARTEd\n Cap: SUPER TRAVEL HAS ENDED :-(\n");
	return;
}

void person(int id, int i, int n)
{
	wait_nil(id, 3); // Ждем остановки корабля
	wait_nil(id, 2); // Ждем опускания трапа
	minus(id, 0, 1); // Если человек зашел на трап, то считаем, что он уже на корабле
	minus(id, 1, 1);
	printf("Person %d: on the ludder\n", i);
	plus(id, 1, 1);
	printf("Person %d: on the ship\n", i);
	return;
}

void wait_nil(int id, int sem_ind)
{
	struct sembuf s = {sem_ind, 0, 0};
	struct sembuf arr[1] = {s};
	semop(id, arr, 1);
}

void minus(int id, int sem_ind, int op)
{
	struct sembuf s = {sem_ind, -op, 0};
	struct sembuf arr[1] = {s};
	semop(id, arr, 1);
}

void plus(int id, int sem_ind, int op)
{
	struct sembuf s = {sem_ind, op, 0};
	struct sembuf arr[1] = {s};
	semop(id, arr, 1);
}