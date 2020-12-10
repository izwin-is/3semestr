#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <time.h>
int count = 0;

void* adder(void* arg)
{
	long long* pi = arg;
	long long i = 0;
	for (i; i < *pi; ++i)
	{
		count++;
	}
}

int main(int argc, char *argv[])
{


	struct timespec tm1, tm2;
	clock_gettime (CLOCK_REALTIME, &tm1);

	long long n = atoi(argv[1]);
	long long m = atoi(argv[2]);
	long long arg = n / m;
	pthread_t mas_id[m];
	int status;

	long long i = 0;
	for (i; i < m; ++i)
	{
		status = pthread_create(&mas_id[i], NULL, adder, &arg);
	}
	i = 0;
	for (i; i < m; ++i)
	{
		pthread_join(mas_id[i], (void**)status);
	}

	printf("Dolzhno %lld\n", n);
	printf("Poluchilos' %lld\n", count);
	clock_gettime (CLOCK_REALTIME, &tm2);
	long long dt = (tm2.tv_sec - tm1.tv_sec) * 1000 +
			 (tm2.tv_nsec - tm1.tv_nsec) / 1000000;
	long long dts = dt / 1000, dtm = dt % 1000;
	printf("Затрачено время: %lld с %lld mс\n", dts, dtm);
	return 0;
}