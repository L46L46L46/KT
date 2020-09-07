#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1024

int ret(int count_n)
{
	if (count_n == 0)
	{
		printf("\n");
	}
	return 0;
}

char* read_argument(char* argument)
{
	char* new_word = malloc(sizeof(char) * SIZE);
	int j = 0;
	for (int i = 0; argument[i] != '\0'; i++)
	{
		if (argument[i] != '"')
		{
			new_word[j] = argument[i];
			j++;
		}
	}
	new_word[j] = '\0';
	return new_word;
}

int main(int argc, char** argv)
{
	int count_n = 0;
	if (argc == 1)
	{
		return ret(count_n);
	}
	for (int i = 1; i < argc; i++)
	{
		char* word = read_argument(argv[i]);
		if (!strcmp(word, "-n") && i == 1)
		{
			count_n = 1;
		}
		else
		{
			if (i < argc - 1)
			{
				printf("%s ", word);
			}
			else
			{
				printf("%s", word);
			}
		}
	}
	ret(count_n);
}
