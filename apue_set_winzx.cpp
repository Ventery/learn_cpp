#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(void)
{
	struct winsize    size;
	size.ws_row=20;
	size.ws_col=20;
	int fd=open("/dev/ttys000",O_RDWR);
	if (ioctl(fd, TIOCSWINSZ, (char *)&size) < 0)
		printf("TIOCGWINSZ error");
}