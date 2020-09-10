#include <stdio.h>
#include <string.h>
int main(int argc, char const *argv[])
{
	int isnewstring = 0;
	int i = 1;
	if (argc == 1)
	{
		isnewstring = 1;
	}
	else
	{
		if (strcmp(argv[1], "-n") == 0)
		{
			i = 2;
		}
		else
		{
			i = 1;
			isnewstring = 1;
		}
		for (i; i < argc; ++i)
		{
			printf("%s", argv[i]);
			if (i != argc - 1)
				printf(" ");
		}
	}
	if (isnewstring == 1)
	{
		printf("\n");
	}
	
	return 0;
}