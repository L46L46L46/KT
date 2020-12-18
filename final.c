#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<dirent.h>
#include<unistd.h>
#include<getopt.h>
#include<string.h>


int counter(char* path, int flag)
{
	int links_counter = 0, other = 0, broken_counter = 0;
	char file;

	DIR* dir = opendir(path);
	struct stat buf;
	struct dirent *next = readdir(dir);
	while (!next)
	{
		int stat_return = lstat(next -> d_name, buf);
		if(S_ISLNK(buf.st_mode))
		{
			int readlink_return = readlink(next -> d_name, &file, 1);
			if (readlink_return == -1 && errno == ENOENT)
			{
				broken_counter++;
			}
			if (readlink_return == -1)
			{
				perror("readlink: ");
				return errno;
			}
			else
			{
				if (flag)
				{
					//stat(next -> d_name, buf);
					printf("%s -> %s\n", next -> d_name, )
				}
				links_counter++;
			}
		}
		else
		{
			other++;
		}

		next = readdir(dir);
	}
	printf("broken: %d, symbolic: %d, other: %d\n", broken_counter, links_counter, other);

}

int main(int argc, char** argv)
{
	int option_flag = 0;
	char* path = argv[1];
	char* name = argv[2];
	
	static struct option long_options[] = {
		{"verbose", required_argument, 'v'},
		{0, 0, 0, 0}
	};
	while((my_option = getopt_long(argc, argv, "v:", long_options, 0)) != -1)
	{
		switch(my_option)
		{
			case 'v':
				option_flag = 1;
				break;
			case '?':
				printf("mycp: invalid key\n");
				return 1;
		}
	}
	return counter(path, option_flag);
}
