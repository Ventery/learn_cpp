#include<iostream>
#include <unistd.h>
#include <sys/select.h>
#include <signal.h>
#include <string.h>
#include <assert.h>

using namespace std;
void handle_alarm(int)
{
	cout<<"handle_alarm"<<endl;
	alarm(3);
}
void add_alarm()//搞一个周期性alarm保持链接
{
	struct sigaction sa;
	memset( &sa, '\0', sizeof( sa ) );
	sa.sa_handler = handle_alarm;
	sa.sa_flags |= SA_RESTART;//很奇怪无法重启select
	sigfillset( &sa.sa_mask );
	assert( sigaction( SIGALRM, &sa, NULL ) != -1 );
}
int main()
{
	fd_set run_set;
	FD_SET(STDIN_FILENO,&run_set);
	add_alarm();
	alarm(1);
	for (;;)
	{
		FD_SET(STDIN_FILENO,&run_set);
		cout<<select(1, &run_set, 0, 0, 0)<<endl;
	}
	return 0;
}
