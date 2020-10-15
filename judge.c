#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

struct msgbuf
{
	long type;
	int msg;
};

int runner (int i, int id, int n)
{
	struct msgbuf im_here;
	im_here.type = 1;
	im_here.msg = i;
	printf("		runner %d here\n", i);
	if (msgsnd(id, &im_here, sizeof(struct msgbuf), 0) == -1)
	{
		return errno;
	}

	struct msgbuf relay;
	if (i == 0)
	{
		if (msgrcv(id, &relay, sizeof(struct msgbuf), 2, 0) == -1)
		{
			return errno;
		}
	}
	else
	{
		if (msgrcv(id, &relay, sizeof(struct msgbuf), 3, 0) == -1)
		{
			return errno;
		}
	}
	printf("		runner %d got a wand", relay.msg);
	
	relay.msg = i + 1;
	printf("		runner %d passed the batton to %d\n", i, relay.msg);
	if (i ==  n - 1)
	{
		relay.type = 4;
		if (msgsnd(id, &relay, sizeof(struct msgbuf), 0) == -1)
		{
			return errno;
		}
	}
	else
	{
		relay.type = 3;
		if (msgsnd(id, &relay, sizeof(struct msgbuf), 0) == -1)
		{
			return errno;
		}
	}

	if (msgrcv(id, &relay, sizeof(struct msgbuf), 5, 0) == -1)
	{
		return errno;
	}
	if (relay.msg == i)
	{
		relay.msg = i + 1;
		if (msgsnd(id, &relay, sizeof(struct msgbuf), 0) == -1)
		{
			return errno;
		}
		printf("		%d go home\n", i);
		return 0;
	}
	printf("RELEASE ME, PLEASE");
	return 1;
}

int judge(int id, int n)
{
	struct msgbuf judge_instruction;
	for (int i = 0; i < n; i++)
	{
		int rcv = msgrcv(id, &judge_instruction, sizeof(struct msgbuf), 0, 0);
		if (rcv == -1)
		{
			return errno;
		}
		printf("	Judge %d here\n", judge_instruction.msg);
	}
	printf("All is here\n");

	printf("You can start run!\n");
//Runners can run
	judge_instruction.msg = 0;
	judge_instruction.type = 2;
	msgsnd(id, &judge_instruction, sizeof(struct msgbuf), 0);
//Runners finish
	msgrcv(id, &judge_instruction, sizeof(struct msgbuf), 4, 0);
	if (judge_instruction.msg != 4)
	{
		printf("Runners didn't run: ");
		return errno;
	}
//Runners can go home
	printf("Runners can go home\n");

	judge_instruction.msg = 0;
	judge_instruction.type = 5;
	if (msgsnd(id, &judge_instruction, sizeof(struct msgbuf), 0) == -1)
	{
		return errno;
	}
	return 0;
}

int main(int argc, char** argv)
{
	struct msqid_ds buf;
	setbuf(stdout, NULL);

	int n = atoi (argv[1]);
	int id = (int)msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0700);
	if (id == -1)
	{
		perror("Getmsg: ");
		return errno;
	}

	pid_t main_pid = getpid();


	int fork_return = fork();
	if (fork_return == -1)
	{
		msgctl(id, IPC_RMID, &buf);
		perror("Fork: ");
		return errno;
	}
	if (fork_return == 0)
	{
		//попытки в ошибки
		if (judge(id, n))
		{
			msgctl(id, IPC_RMID, &buf);
			perror("Error of receiving a report about the arrival of the judge: ");
			return errno;
		}
		exit(0);
	}
	for (int i = 0; i < n; i++)
	{
		fork_return = fork();
		if (fork_return == -1)
		{
			msgctl(id, IPC_RMID, &buf);
			perror("Fork: ");
			return errno;
		}
		if (!fork_return)
		{
			if (runner(i, id, n))
			{
				msgctl(id, IPC_RMID, &buf);
				perror("Error of receiving a report about the arrival of the runner: ");
				return errno;
			}
			exit(0);
		}
	}

	while(wait(&main_pid) != -1)
	{
		//
	}
	msgctl(id, IPC_RMID, &buf);

}
