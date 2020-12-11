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
#include <string.h>

struct s {
	long type;
	char str[10];
};

 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

// void writer(char *str);

int main(int argc, char const *argv[])
{
	char *path = "single.c";
	key_t key = ftok(path, 8);
	int id = msgget(key, (IPC_CREAT | 0666));


	pid_t pid = getpid();


	struct msqid_ds buf;
	msgctl(id, IPC_STAT, &buf);

	if (buf.msg_stime == 0)
	{
		struct s message;
		message.type = 1;
		char str_pid[10];
		itoa(pid, &str_pid);
		strncpy(message.str, str_pid, 10);
		msgsnd(id, &message, 10, 0);


		char str[] = "Hello, world!\n";
		int i = 0;
		for (i; i < sizeof(str); i++)
		{
			usleep(500000);
			write(1, str + i, 1);
		}
		msgctl(id, IPC_STAT, &buf);

		msgctl(id, IPC_RMID, NULL);
	}
	else
	{
		struct s message;
		pid_t hello_pid;
		msgrcv(id, &message, 10, 1, 0);

		if (kill(atoi(message.str), 0) < 0)
		{
			struct s message;
			message.type = 1;
			char str_pid[10];
			itoa(pid, &str_pid);
			strncpy(message.str, str_pid, 10);
			msgsnd(id, &message, 10, 0);


			char str[] = "Hello, world!\n";
			int i = 0;
			for (i; i < sizeof(str); i++)
			{
				usleep(500000);
				write(1, str + i, 1);
			}



			return 0;
		}




		char str[] = "Goodbye, world!\n";
		int i = 0;
		for (i; i < sizeof(str); i++)
		{
			usleep(500000);
			write(1, str + i, 1);
		}
		msgctl(id, IPC_RMID, NULL);
	}

	return 0;
}

// void writer(char *str)
// {
// 	int i = 0;
// 	for (i; i < sizeof(str); i++)
// 	{
// 		usleep(500000);
// 		write(1, str + i, 1);
// 	}
// }