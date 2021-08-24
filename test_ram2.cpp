#include<iostream>
#include<unistd.h>
#define bs 1024*1024*1024
using namespace std;
int main()
{
	int a,b,c;
	int * d,* e,* f;
	cout<<&a<<" "<<&b<<" "<<&c<<endl;
	d=new int;
	e=new int;
	f=new int;
	cout<<d<<" "<<e<<" "<<f<<endl;
	void *k;
	k=malloc(bs);
	for (int i=0;i<bs;i++)
	{
		if (i%(bs/20)==0) 
		{
			sleep(2);
			cout<<i/(bs/20)<<"/"<<20<<endl;
		}	
		*((char *)k+i)='k';
	}
	sleep(100000);
	return 0;
}
