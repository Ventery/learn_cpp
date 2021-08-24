#include<iostream>
#include<memory>
using namespace std;
int main()
{
	shared_ptr<int> p1(new int(4396));
	cout<<*p1<<endl;
	auto p2=std::move(p1);
	cout<<"*p2:"<<*p2<<endl;
	cout<<"p1:"<<p1<<endl;
	cout<<"*p1:"<<*p1<<endl;
	return 0;
}
