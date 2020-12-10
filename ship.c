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

void captain(int id, int num_travellings);
void person(int id, int i, int n, int num_travellings);
void wait_nil(int id, int sem_ind1, int sem_ind2);
void minus(int id, int sem_ind1, int sem_ind2);
void plus(int id, int sem_ind1, int sem_ind2);


int main(int argc, char const *argv[])
{
	// setbuf(stdout, NULL);
	int n = atoi(argv[1]), k = atoi(argv[2]), m = atoi(argv[3]), num_travellings = atoi(argv[4]);
	int id = semget(IPC_PRIVATE, 5, (IPC_CREAT | IPC_EXCL | 0700));
	int i = 1;
	semctl(id, 0, SETVAL, k); // ship
	semctl(id, 1, SETVAL, m); // ludder
	semctl(id, 2, SETVAL, 1); // ludder down
	semctl(id, 3, SETVAL, 1); // ship is not moving
	semctl(id, 4, SETVAL, 1); // Travelling is not ended

	pid_t pid;

	for (i; i < n + 1; ++i)
	{
		pid = fork();
		if (pid == 0)
		{
			person(id, i, n, num_travellings);
			return 0;
		}
	}
	pid = fork();
	if (pid == 0) // Child
	{
		captain(id, num_travellings);
		return 0;
	}

	int stat;
	while (wait(&stat) != -1);
	semctl(id, IPC_RMID, NULL);
	return 0;
}
void captain(int id, int num_travellings)
{
	int i = 0;
	for (i; i < num_travellings; i++)
	{
		printf("Cap: Travelling number %d\n", i + 1);
		printf("Cap: Approaching to the port\nCap: The ship has stopped\nCap: The ludder is down\n");
		// sync();
		minus(id, 2, 3); // Швартуем корабль и опускаем трап 


		wait_nil(id, 0, -1); // Ждем заходящих людей
		usleep(100); //  Иначе 65 строка может напечататься раньше, чем сообщение о том, то человек зашёл на корабль
		plus(id, 2, 3); // Поднимаем трап, начинаем движение
		printf("Cap: SUPER TRAVEL HAS STARTED\nCap: SUPER TRAVEL HAS ENDED :-(\n");

		minus(id, 2, 3); // Швартуем корабль и опускаем трап
		minus(id, 4, -1); // Заканчиваем путешествие
		
		plus(id, 4, -1); // Начало нового путешествия
	}
	return;
}

void person(int id, int i, int n, int num_travellings)
{

	wait_nil(id, 2, 3); // Ждем остановки корабля и опускания трапа

	minus(id, 0, 1); // Зашёл на трап, при этом мы считаем, что он уже на корабле

	if (semctl(id, 4, GETVAL) == 0) // Если поездка началась, а пассажир не сел,
	{                               // то он уже не сядет на корабль 
		return;
	}

	printf("Person %d: on the ludder and go to the ship (shipval = %d)\n", i, semctl(id, 0, GETVAL));


	// if (semctl(id, 4, GETVAL) == 0) // Если поездка началась, а пассажир не сел,
	// {                               // то он уже не сядет на корабль 
	// 	return;
	// }

	plus(id, 1, -1); // Сошёл с трапа на корабль
	printf("Person %d: on the ship\n", i);

	

	wait_nil(id, 2, 3); // Ждем остановки корабля и опускания трапа
	wait_nil(id, 4, -1);  // Ждём конца поездки 
	plus(id, 0, -1); // Сошли с корабля на трап
	minus(id, 1, -1);
	printf("Person %d: on the ludder and go to the land\n", i);

	plus(id, 1, -1); // Сошёл на землю
	printf("Person %d: on the land\n", i);
	return;
}

void wait_nil(int id, int sem_ind1, int sem_ind2)
{

	struct sembuf s1 = {sem_ind1, 0, 0};
	if (sem_ind2 >= 0)
	{
		struct sembuf s2 = {sem_ind2, 0, 0};
		struct sembuf arr[2] = {s1, s2};
		semop(id, arr, 2);
	}
	else
	{
		struct sembuf arr[1] = {s1};
		semop(id, arr, 1);
	}
}

void minus(int id, int sem_ind1, int sem_ind2)
{
	struct sembuf s1 = {sem_ind1, -1, 0};
	if (sem_ind2 >= 0)
	{
		struct sembuf s2 = {sem_ind2, -1, 0};
		struct sembuf arr[2] = {s1, s2};
		semop(id, arr, 2);
	}
	else
	{
		struct sembuf arr[1] = {s1};
		semop(id, arr, 1);
	}
}

void plus(int id, int sem_ind1, int sem_ind2)
{
	struct sembuf s1 = {sem_ind1, 1, 0};
	if (sem_ind2 >= 0)
	{
		struct sembuf s2 = {sem_ind2, 1, 0};
		struct sembuf arr[2] = {s1, s2};
		semop(id, arr, 2);
	}
	else
	{
		struct sembuf arr[1] = {s1};
		semop(id, arr, 1);
	}
}