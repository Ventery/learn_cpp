#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string>
using namespace std;
int main()
{
	int socket_pair[2];
	socketpair( PF_UNIX, SOCK_STREAM, 0, socket_pair );
	int pid = fork();
	char rs[1024];
	memset(rs, 0, 1024);
	if (pid == 0)
	{
		close(STDOUT_FILENO);
		dup(socket_pair[0]);

		close(STDIN_FILENO);
		dup(socket_pair[0]);
		execl("/bin/bash", "/bin/bash -l", 0 );
		return 0;
	}
	char *msg = "date";
	write(socket_pair[1], msg, strlen(msg));
	write(socket_pair[1], "\n", 1);
	msg="exit";
	write(socket_pair[1], msg, strlen(msg));
	write(socket_pair[1], "\n", 1);
	read(socket_pair[1], rs, 1024);
	cout << rs << endl;
	return 0;
}
