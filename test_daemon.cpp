#include<iostream>
#include <unistd.h>
#include <stdlib.h>
using namespace std;
int main()
{
	daemon(0,0);
	sleep(1000);
	return 0;
}
