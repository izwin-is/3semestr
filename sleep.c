#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>



int main(int argc, char *argv[])
{
	int i = 1, n = argc - 1;
	pid_t pid;
	for (i; i < n + 1; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			usleep(atoi(argv[i]) * 5000);
			printf("%s ", argv[i]);
			return 0;
		}
	}
	i = 1;
	int stat;
	while (wait(&stat) != -1)
	{
		usleep(1);
	}
	printf("\n");

	return 0;
}