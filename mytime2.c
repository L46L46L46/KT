#include<unistd.h>
#include<ctype.h>
#include<time.h>
#include<errno.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<getopt.h>
#include<fcntl.h>
#define SIZE 1024

int read_text_from_input(int* bytes, int* words, int* rows, int pipefd)
{
	int rread;
	char buf;
	int /*bytes = 0, words = 0, rows = 0,*/ last_is_word = 1;
	while((rread = read(pipefd, &buf, 1)) >= 0)
	{
		if (!isalnum(buf))
		{
			if(last_is_word)
			{
				(*words)++;
			}
			last_is_word = 0;
		}
		else
		{
			last_is_word = 1;
		}
		if (buf == '\n')
		{
			(*rows)++;
			last_is_word = 0;
		}
		if (rread == 0)
		{
			if (last_is_word)
			{
				(*words)++;
			}
			(*rows)++;
			break;
		}
		(*bytes)++;
	}
	return rread;
}

int main(int argc, char** argv)
{
	//Getting option
	int option_flag = 0;
	int my_option;
	static struct option long_options[] = { 
		{"wc",required_argument, 0, 'w'},
		{0, 0, 0, 0}
	};

	while ((my_option = getopt_long(argc, argv, "+w:", long_options, 0)) != -1)
	{
		switch (my_option)
		{
			case 'w':
				option_flag = option_flag | (01);
				break;
//			case '?':
//				write(1, "mytime: invalid key\n", strlen("mycp: invalid key\n"));
//				return 1;
		}
	}

	while ((my_option = getopt(argc, argv, "+w:")) != -1)
	{
		switch (my_option)
		{
			case 'w':
				option_flag = option_flag | (01);
				break;
//			case '?':
//				write(1, "mytime: invalid key\n", strlen("mytime: invalid key\n"));
//				return 1;	
		}
	}
	for(int i = 0; i < argc; i++)
	{
		printf("%s\n", argv[i]);
	}
	if (option_flag)
	{
		int pipefd[2];
		pid_t main_pid = getpid();
	
		if (pipe(pipefd) == -1)
		{
			perror("Pipe: ");
			return errno;
		}
	
		//Make child process
		pid_t fork_return = fork();
		if (fork_return == -1)
		{
			perror("Fork: ");
			return errno;
		}
	
		//Transfer control the program
		if (fork_return == 0)
		{
			close(1);
			dup(pipefd[1]);
			for (int i = 0; i < argc - 1; i++)
			{
				argv[i] = argv[i + 1];
			}
			argv[argc - 1] = NULL;
			int execvp_return = execvp(argv[1], argv + 1);
			if (execvp_return == -1)
			{
				perror("Exec: ");
				return errno;
			}
			exit(0);
		}
	
		while(wait(&main_pid) != -1)
		{
			//
		}
		close(pipefd[1]);
	
		//Counting bytes, words, rows
		int bytes = 0, words = 0, rows = 0;
		if (read_text_from_input(&bytes, &words, &rows, pipefd[0]) < 0)
		{
			perror("read: ");
			return errno;
		}
		printf("bytes=%d, words=%d, rows=%d", bytes, words, rows);
		return 0;
	}
//If there are no argumets, it work like mytime
	struct timespec t1, t2;
	//Getting start time
	if (clock_gettime(CLOCK_MONOTONIC, &t1) == -1)
	{
		perror("Gettime: ");
		return errno;
	}

	pid_t main_pid = getpid();
	//Make child process
	pid_t fork_return = fork();
	if (fork_return == -1)
	{
		perror("Fork: ");
		return errno;
	}

	if (fork_return == 0)
	{
		for (int i = 0; i < argc - 1; i++)
		{
			argv[i] = argv[i + 1];
		}
		argv[argc - 1] = NULL;
		//Transer control the program
		if (execvp(argv[0], argv) == -1)
		{
			perror("Exec: ");
			return errno;
		}
		
		exit(0);
	}

	while(wait(&main_pid) != -1)
	{
		//
	}
	//Getting finish time
	if (clock_gettime(CLOCK_MONOTONIC, &t2) == -1)
	{
		perror("Gettime: ");
		return errno;
	}
	printf("%ld.%ldms\n", (t2.tv_nsec - t1.tv_nsec)/1000000, (t2.tv_nsec - t1.tv_nsec)%1000000);
	return 0;
}
