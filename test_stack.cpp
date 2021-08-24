#include<iostream>
using namespace std;
struct A
{
	A(){cout<<"constructor!"<<endl;}
	A(A& oa){cout<<"copy cons"<<endl;}
private:
	A(A&& oa){cout<<"move cons"<<endl;}
public:
	A& operator=(A& a){cout<<"copy opter"<<endl;return *this;}
	A& operator=(A&& a){cout<<"move opter"<<endl;return *this;}
	~A(){cout<<"destructor"<<endl;}
};
A func()
{
	A tmp;
	cout<<&tmp<<endl;
	return tmp;
}
int main()
{
	func();
//	A main_tmp=func();
//	cout<<&main_tmp<<endl;
	return 0;
}
