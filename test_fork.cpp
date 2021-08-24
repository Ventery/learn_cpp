#include <iostream>
#include <unistd.h>
using namespace std;
int main()
{
	int pid;
	pid=fork();
	if (pid==0)
	{
	setsid();
	}
	sleep(1000);
	return 0;
}
