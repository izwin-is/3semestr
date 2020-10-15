#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct s {
	long type;
	char str[1];
};

void judje(int msg_id, int n);
void runner(int i, int msg_id, int n);

int main(int argc, char *argv[])
{
	setbuf(stdout, NULL);
	int n = atoi(argv[1]);
	int id = msgget(IPC_PRIVATE, (IPC_CREAT | IPC_EXCL | 0700));
	pid_t pid;
	int i = 1;

	pid = fork();
	if (pid == 0) // Child
	{
		judje(id, n);
		return 0;
	}
	for (i; i < n + 1; ++i)
	{
		pid = fork();
		if (pid == 0)
		{
			runner(i, id, n);
			return 0;
		}
	}

	int stat;
	while (wait(&stat) != -1);
	msgctl(id, IPC_RMID, NULL);
	return 0;
}


void judje(int msg_id, int n)
{

	struct s message, buf;
	message.type = n + 1;
	int i = 1;
	for (i; i < n + 1; ++i)
	{
		msgrcv(msg_id, &buf, 1, i, 0);
		printf("runner %d came\n", i);
	}
	printf("--all runners came--\n");


	msgsnd(msg_id, &message, 1, 0); // The stick starts

	msgrcv(msg_id, &buf, 1, 2 * n + 1, 0); // All the runners finished
	printf("--all the runners finished--\n--you can leave the competition--\n");
	message.type = 2 * n + 2;
	i = 1;
	for (i; i < n + 1; ++i) // Saying goodbye
	{
		msgsnd(msg_id, &message, 1, 0);
	}

}

void runner(int i, int msg_id, int n)
{
	struct s message, buf;
	message.type = i;
	msgsnd(msg_id, &message, 1, 0);

	msgrcv(msg_id, &buf, 1, n + i, 0); // Recieve the stick
	printf("runner %d finished\n", i);
	message.type = n + i + 1;
	msgsnd(msg_id, &message, 1, 0); // Give the stick to the following
	msgrcv(msg_id, &buf, 1, 2 * n + 2, 0); // Wait for a leaving
	printf("runner %d left the competition\n", i);
}