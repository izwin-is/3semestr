#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <getopt.h>

int writer(int from, int to);

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("operands missed\n");
		return 0;
	}
	
	int keys[3] = {0};
	const char* short_options = "vif";

		const struct option long_options[] = {
			{"verbose",no_argument,NULL,'v'},
			{"interactive",no_argument,NULL,'i'},
			{"force",no_argument,NULL,'f'},
			{NULL,0,NULL,0}
		};

		int rez;
		int option_index;

		while ((rez=getopt_long(argc,argv,short_options,
			long_options,&option_index))!=-1)
		{

			switch(rez)
			{
				case 'v': {
					keys[0] = 1;
					break;
				};
				case 'i': {
					keys[1] = 1;
					break;
				};
		
				case 'f': {
					keys[2] = 1;
					break;
				};
				case '?': default: {
					printf("found unknown option\n");
					break;
				};
			};
		};
		printf("%d, %d, %d", keys[0], keys[1], keys[2]);


	if (1)
	{
		int i = 0;
		for (i; i < argc; i++)
		{
			
			int fd_read = open(argv[1], O_RDONLY);
			int fd_write = open(argv[2], O_WRONLY|O_CREAT, S_IREAD);
			writer(fd_read, fd_write);
		}

	}
	return 0;
}

int writer(int from, int to)
{
	if (from < 0)
	{
		perror("mycat");
		return 0;

	}
	else
	{
		size_t size = 4096;
		char buff[4096];
		ssize_t real_size = read(from, buff, size);
		ssize_t already_written = 0;
		while (already_written != real_size)
			{
				already_written += write(to, buff + already_written,
									  real_size - already_written);
			}
		return real_size;
	}

}