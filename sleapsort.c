#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		write(1, "\n", strlen("1"));
	}

	int parent_pid = getpid();

	for (int i = 1; i < argc; i++)
	{
		pid_t fork_return = fork();
		if (fork_return == -1)
		{
			perror("Error: ");
			return errno;
		}
		if (fork_return == 0)
		{
			int x = atoi(argv[i]);
			if (usleep(500 * x) == -1)
			{
				perror("Error: ");
				return errno;
			}
			printf("%d", x);
			exit(0);
		}
	}

	while(wait(&parent_pid) != -1)
	{
		//
	}
	printf("\n");

}
