#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>

int from_input_to_stdout(int from, int to, int file_or_stdin)
{
	size_t buf_size = 4096;
	char* buf = malloc(buf_size);
	int rread = read(from, buf, buf_size);
	if (rread < 0)
	{
		perror("Error: ");
		free(buf);
		close(from);
		return errno;
	}
	while(rread > 0 && file_or_stdin == 0)
	{
		int wwrite = write(to, buf, rread);
		if (wwrite != rread)
		{
			perror("Error: ");
			free(buf);
			close(from);
			return errno;
		}
		rread = read(from, buf, buf_size);
		file_or_stdin = from;
	}
	free(buf);
	close(from);
	if (rread < 0)
	{
		perror("Error: ");
		return errno;
	}
	return 0;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{

		return from_input_to_stdout(0, 1, 0);
	}
	for (int i = 1; i <= argc; i++)
	{
		int from = open(argv[i], O_RDONLY, 0777);
		if (from < 1)
		{
			perror("Error: ");
			return errno;
		}
		int ret = from_input_to_stdout(from, 1, 0);
		if (ret != 0)
		{
			return ret;
		}
	}
}
