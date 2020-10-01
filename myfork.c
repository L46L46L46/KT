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

int main(int argc, char** argv)
{

	//Reading
	if (argc != 2)
	{
		return Error();
	}

	int n = string_to_int(argv[argc - 1]);
	for(int i = 0; i < n; i++)
	{
		pid_t fork_answer = fork();
		if(fork_answer == -1)
		{
			return Error();
		}
		
		if (fork_answer == 0)
		{
			printf("Child PID=%d ", getpid());
			printf("Child PPID=%d\n", getppid());
			exit(0);
		}
	}
}
