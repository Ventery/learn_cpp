#include<iostream>
#include<exception>
using namespace std;
int main()
{
	try{
	int a = 1;
	int b = 0;
	int c = 4396;
	cout<<1/b<<endl;
	} catch (exception e)
	{
		cout<<e.what()<<endl;
		cout<<123<<endl;
	}
	return 0;
}
