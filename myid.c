#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<pwd.h>
#include<grp.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
	int uid;
	char* uid_name;
	int gid;
	char* gid_name;
	int groups = 100;	

	gid_t* list_groups = malloc(64 * sizeof(gid_t));

	if (argc == 1)
	{
		uid = getuid();
		uid_name = getpwuid(uid) -> pw_name;
	}
	if (argc == 2)
	{
		uid_name = argv[1];
		uid = (getpwnam(uid_name)) -> pw_uid;
	}

		gid_name = uid_name;
		gid = (getgrnam(gid_name)) -> gr_gid;
		getgrouplist (uid_name, getpwnam(uid_name) -> pw_gid, list_groups, &groups);

	printf("uid=%d(%s) ", uid, uid_name);
	printf("gid=%d(%s) ", gid, gid_name);
	printf("group=%d(%s)", list_groups[0], getgrgid(list_groups[0]) -> gr_name);

	for (int i = 1; i < groups; i++)
	{
		if (i < groups)
		{
			printf(",");
		}
		printf("%d(%s)", list_groups[i], (char*) getgrgid(list_groups[i]) -> gr_name);
	}
	printf("\n");
	return 0;
}
