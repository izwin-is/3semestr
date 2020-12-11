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
#include <sys/wait.h>

struct s {
	long type;
	char str[1];
};

int main(int argc, char const *argv[])
{
	char *path = "single.c";
	key_t key = ftok(path, 1);
	int id = msgget(key, (IPC_CREAT | 0666));

	struct msqid_ds buf;
	msgctl(id, IPC_STAT, &buf);
	// printf("%ld", buf.msg_stime);
	if (buf.msg_stime == 0)
	{
		struct s message;
		message.type = 1;
		msgsnd(id, &message, 1, 0);

		char str[] = "H\ne\nl\nl\no\n,\n \nw\no\nr\nl\nd\n";
		int i = 0;
		for (i; i < sizeof(str); i++)
		{
			usleep(500000);
			printf("%c", str[i]);
		}


		msgctl(id, IPC_STAT, &buf);
		// printf("%ld", buf.msg_stime);

		msgctl(id, IPC_RMID, NULL);
	}
	else
	{
		printf("Goodbye, world\n");
		msgctl(id, IPC_RMID, NULL);
	}


	// int program_type = 0;




	
	return 0;
}