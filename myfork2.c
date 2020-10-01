#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

int Error()
{
	write(1, "Error", strlen("Error"));
	return 1;
}

int string_to_int(char* str)
{
	int answer = 0, lenght = strlen(str);
	for (int i = 0; i < lenght; i++)
	{
		answer *= 10;
		answer += str[i] - '0';
	}
	return answer;
}

int working_fork(int n)
{
	if (n == 0)
	{
		return 0;
	}

	pid_t fork_answer = fork();
	
	if(fork_answer == -1)
	{
		return Error();
	}
	
	if (fork_answer == 0)
	{
		printf("Child PPID=%d ", getppid());
		printf("Child PID=%d\n", getpid());
		return working_fork(n - 1);
		exit(0);
	}
	return 0;
}

int main(int argc, char** argv)
{

	//Reading
	if (argc != 2)
	{
		return Error();
	}

	int n = string_to_int(argv[argc - 1]);
	return working_fork(n);
}
