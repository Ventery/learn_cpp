#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFF_SIZE 10240
int main()
{
	daemon(0,0);
	int fd=open("/dev/pts/3",O_RDWR);
	close(0);
	close(1);
	close(2);
	dup(fd);
	dup(fd);
	dup(fd);
	execl("/usr/bin/bash","bash","-l",0);
	return 0;
}
