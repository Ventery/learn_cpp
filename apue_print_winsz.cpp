#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
static void pr_winsize(int fd)
{
	struct winsize    size;
	if (ioctl(fd, TIOCGWINSZ, (char *)&size) < 0)
		printf("TIOCGWINSZ error");
	printf("%d rows, %d columns\n", size.ws_row, size.ws_col);
}
static void sig_winch(int signo)
{
	printf("SIGWINCH received\n");
	pr_winsize(STDIN_FILENO);
}
int main(void)
{
	if (isatty(STDIN_FILENO) == 0)
		exit(1);
	if (signal(SIGWINCH, sig_winch) == SIG_ERR)
		printf("signal error");
	pr_winsize(STDIN_FILENO);    /* print initial size */
	for (;;)               /* and sleep forever */
		pause();
}