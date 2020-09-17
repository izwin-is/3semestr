#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		uid_t uid = getuid();
		gid_t gid = getgid();
		int n, i;
		int size = 20;
		gid_t list[20];
		printf("uid=%d(%s) ", uid, getpwuid(uid)->pw_name);
		printf("gid=%d(%s) ", gid, getgrgid(gid)->gr_name);
		printf("groups=%d(%s),", gid, getgrgid(gid)->gr_name);

		n = getgroups(size, list);
		for (i = 0; i < n - 1; i++)
		{
			printf("%d(%s)%s", list[i],getgrgid(list[i])->gr_name, i != n - 2 ? "," : "");
		}
		printf("\n");
	}
	if (argc == 2)
	{
		char *name;
		name = argv[1];
		uid_t uid = getpwnam(name)->pw_uid;
		gid_t gid = getpwnam(name)->pw_gid;
		printf("uid=%d(%s) gid=%d(%s) groups=", uid, name, gid, name);
		int size = 20, i;
		gid_t groups_id[size];
		size = getgrouplist(name, gid, groups_id, &size);
		for (i = 0; i < size; ++i)
		{
			printf("%d(%s)%s", groups_id[i],
				   getgrgid(groups_id[i])->gr_name, i == size - 1 ? "\n" : ",");
		}
	}
	return 0;
}