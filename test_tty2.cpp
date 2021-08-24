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

using namespace std;
#define BUF_SIZE 10240
#define EPOLL_SIZE 1000
#ifdef __linux__
#define TTY_PATH            " /dev/pts/0"
#define STTY_US             "stty raw -echo -F "
#define STTY_DEF            "stty -raw echo -F "
#elif __APPLE__
#define TTY_PATH            " 1234"
#define STTY_PREFIX         "stty -f"
#define STTY_US_SUFFIX      " raw -echo"
#define STTY_DEF_SUFFIX     " -raw echo"
#endif
void handle_int(int sig)
{
	if (sig==SIGINT)
	printf("SIGINT received\n");
	else if (sig==SIGTERM) 
	printf("SIGTERM received\n");

    #ifdef __linux__
        system(STTY_DEF TTY_PATH);
    #elif __APPLE__
        system(STTY_PREFIX TTY_PATH STTY_DEF_SUFFIX);
    #endif
    exit(0);
}
int main()
{
	signal(SIGINT,handle_int);
	signal(SIGTERM,handle_int);

	int ptmx_fd=posix_openpt(O_RDWR);
	unlockpt(ptmx_fd);
	string pts_path=string(ptsname(ptmx_fd));//slave path
#ifdef __linux__
    system(STTY_US TTY_PATH);
#elif __APPLE__
    system(STTY_PREFIX TTY_PATH STTY_US_SUFFIX);
#endif
	char buf[BUF_SIZE];
	int str_len;
	int ret;
	int epfd, event_cnt;
	struct epoll_event *ep_events;
	struct epoll_event event_stdin,event_stdout,event_ptmx;
	epfd=epoll_create(EPOLL_SIZE);
	ep_events=(epoll_event*)malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	event_ptmx.events = EPOLLIN;
	event_ptmx.data.fd=ptmx_fd;	
	epoll_ctl(epfd, EPOLL_CTL_ADD, ptmx_fd, &event_ptmx);

	event_stdin.events = EPOLLIN ;
	event_stdin.data.fd=STDIN_FILENO;	
	ret=epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event_stdin);

	for (;;)
	{
		int event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		for(int i=0; i<event_cnt; i++)
		{
			//printf("fd %d actived\n",ep_events[i].data.fd);
			if(ep_events[i].data.fd==STDIN_FILENO)
			{
				str_len=read(ep_events[i].data.fd, buf, BUF_SIZE);//这里的read和下面write是否可以使用零拷贝？（暂时使用共享内存
				write(ptmx_fd,buf,str_len);
			}
			else if (ep_events[i].data.fd==ptmx_fd)
			{
				str_len=read(ep_events[i].data.fd, buf, BUF_SIZE);
				if (str_len < 0)
				{
					if (errno==EINTR) continue;
					else {
							printf("bash exit!\n");
							epoll_ctl(epfd, EPOLL_CTL_DEL, ptmx_fd, NULL);
		                    #ifdef __linux__
		                        system(STTY_DEF TTY_PATH);
		                    #elif __APPLE__
		                        system(STTY_PREFIX TTY_PATH STTY_DEF_SUFFIX);
		                    #endif
							close(ptmx_fd);
							return 0;
						}
				}
				write(STDOUT_FILENO,buf,str_len);
			}
		}
	}

	return 0;
}
