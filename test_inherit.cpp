#include<iostream>
using namespace std;

class A 
{
public:
	A(){
		check();
	}

	void init()
	{
		check();
	}
	
	virtual void check()
	{
		cout<<"A check!"<<endl;
	}
};

class B : public A
{
public :
	virtual void check()
	{
		cout<<"B check!"<<endl;
	}
};

int main()
{
	A a;
	B b;
	a.init();
	b.init();
	return 0;
}
