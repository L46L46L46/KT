#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>

int from_input_to_stdout(int from, int to)
{
	size_t buf_size = 4096;
	char* buf = malloc(buf_size);	
	int rread;
	while((rread = read(from, buf, buf_size)) > 0)
	{
		int wwrite = write(to, buf, rread);
		if (wwrite != rread)
		{
			free(buf);
			close(from);
			printf("Error: ");
			return errno;
		}
	}
	free(buf);
	if (rread < 0)
	{		
		close(from);
		printf("Error: ");
		return errno;
	}
	return 0;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{

		return from_input_to_stdout(0, 1);
	}
	for (int i = 1; i < argc; i++)
	{
		int from = open(argv[i], O_RDONLY, 0777);
		if (from < 1)
		{
			perror("Error: ");
			return errno;
		}
		int ret = from_input_to_stdout(from, 1);
		if (ret != 0)
		{
			return ret;
		}
		close(from);
	}
}
