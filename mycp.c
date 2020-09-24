#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<getopt.h>

int my_errno()
{
	printf("Error: ");
	return errno;
}

//Getting user answer about the need to write to the file
int read_user_answer()
{
	int buf_size = 1, answer = 0;
	char* buf = malloc(buf_size);
	int rread = read(0, buf, buf_size);
	if (buf[0] == '\n')
	{
		free(buf);
		return 0;
	}
	if (buf[0] == 'y' || buf[0] == 'Y')
	{
		answer = 1;
	}
	while((rread = read(0, buf, buf_size) > 0) && buf[0] != '\n')
	{
		//
	}
	free(buf);
	return answer;
}

//Cheking for file existense
int file_existence(const char* file_name)
{
	struct stat buf;
	return stat(file_name, &buf);
}

int main(int argc, char** argv)
{
	//Read option -f -i and -v
	int option_flag = 0;
	int my_option;
	static struct option long_options[] = { 
		{"forse", required_argument, 0, 'f'},
		{"interactive", required_argument, 0, 'i'},
		{"verbose", required_argument, 0, 'v'},
		{0, 0, 0, 0}
	};

	while ((my_option = getopt_long(argc, argv, "f:i:v:", long_options, 0)) != -1)
	{
		switch (my_option)
		{
			case 'f':
				option_flag = option_flag | (0001);
				break;
			case 'i':
				option_flag = option_flag | (0010);
				break;
			case 'v':
				option_flag = option_flag | (0100);
				break;
			case '?':
				write(1, "mycp: invalid key\n", strlen("mycp: invalid key\n"));
				return 1;
		}
	}

	while ((my_option = getopt(argc, argv, "fiv")) != -1)
	{
		switch (my_option)
		{
			case 'f':
				option_flag = option_flag | (0001);
				break;
			case 'i':
				option_flag = option_flag | (0010);
				break;
			case 'v':
				option_flag = option_flag | (0100);
				break;
			case '?':
				write(1, "mycp: invalid key\n", strlen("mycp: invalid key\n"));
				return 1;	
		}
	}

	//Checking for availability input and output
	if (argc < 3)
	{
		char* error_file = "mycp: missing file operand\n";
		write(1, error_file, strlen(error_file));
		return 1;
	}
	//Open file for read
	int from = open(argv[argc - 2], O_RDONLY, 0000); //file number to read
	if (from == -1)
	{
		return my_errno();
	}

	//Open file for write
	int to; //file number to write

	//If -v if option
	int message_about_completed = 0;
	if (option_flag &0100)
	{
		message_about_completed = 1;
	}

	//If -i is option
	if (option_flag & 0010)
	{
		to = open(argv[argc - 1], O_WRONLY | O_CREAT | O_EXCL, 0644);
		if (to == -1)
		{
			//Does the file exist?
			if (file_existence(argv[argc - 1]) == -1)
			{
				return my_errno();
			}
			//Answer about rewrite
			char* are_you_shure = "mycp: rewrite '";
			write(1, are_you_shure, strlen(are_you_shure));
			write(1, argc[argv - 1], strlen(argc[argv - 1]));
			write(1, "'? ", strlen("'? "));
			
			int buf = read_user_answer();

			if (buf == 0)
			{
				return 0;
			}
			to = open(argv[argc - 1], O_WRONLY | O_CREAT);
		}
	}

	//If -f is option
	if (option_flag & 0001)
	{
		to = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
		if (to == -1)
		{
			if (remove(argv[argc - 1]) == -1)
			{
				return my_errno();
			}
			to = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
		}
	}
	//If there are no options for delete
	if( !(option_flag & 0001) )
	{
		to = open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
		if (to == -1)
		{
			return my_errno();
		}
	}

//Copy from file "from" to file "to"
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
			close(to);
			return my_errno();
		}
	}
	free(buf);
	close(from);
	close(to);
	if (rread < 0)
	{
		return my_errno();
	}
	if (message_about_completed)
	{
		write(1, "'", 1);
		write(1, argc[argv - 2], strlen(argc[argv - 2]));
		write(1, "' -> '", strlen("' -> '"));
		write(1, argc[argv - 1], strlen(argc[argv - 1]));
		write(1, "'\n", strlen("'\n"));
	}
}
