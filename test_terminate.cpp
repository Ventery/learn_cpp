#include<iostream>
using namespace std;
class A
{
	public:
	~A()
	{
	cout<<"fucked!"<<endl;
	}
};
int main()
{
	shared_ptr<A> ip(new A());
	{
		A a;
		shared_ptr<A> ip2(new A());
	}
	for (;;)
	{}
	return 0;
}
