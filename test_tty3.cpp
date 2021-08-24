#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <signal.h>
#include <string>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
#define BUF_SIZE 10240
#define EPOLL_SIZE 1000
int main()
{
	int fd=open("/dev/pts/4",O_RDWR);
	
	char buf[BUF_SIZE];
	int str_len;
	int ret;
	int epfd, event_cnt;
	struct epoll_event *ep_events;
	struct epoll_event event_pts;
	epfd=epoll_create(EPOLL_SIZE);
	ep_events=(epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	event_pts.events = EPOLLIN;
	event_pts.data.fd=fd;	
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event_pts);

	for (;;)
	{
		int event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if (event_cnt) printf("Something happened!\n");
	}

	return 0;
}
