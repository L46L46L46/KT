#include<unistd.h>
#include<time.h>
#include<errno.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv)
{
	struct timespec t1, t2;
	if (clock_gettime(CLOCK_MONOTONIC, &t1) == -1)
	{
		perror("Error: ");
		return errno;
	}

	pid_t main_pid = getpid();

	pid_t fork_return = fork();
	if (fork_return == -1)
	{
		perror("Error: ");
		return errno;
	}

	if (fork_return == 0)
	{
		for (int i = 0; i < argc - 1; i++)
		{
			argv[i] = argv[i + 1];
		}
		argv[argc - 1] = NULL;

		int exec_return = execvp(argv[0], argv);
		if (exec_return == -1)
		{
			perror("Error: ");
			return errno;
		}
		
		exit(0);
	}

	while(wait(&main_pid) != -1)
	{
		//
	}

	if (clock_gettime(CLOCK_MONOTONIC, &t2) == -1)
	{
		perror("Error: ");
		return errno;
	}
	printf("%ld.%ldms\n", (t2.tv_nsec - t1.tv_nsec)/1000000, (t2.tv_nsec - t1.tv_nsec)%1000000);
	return 0;
}
