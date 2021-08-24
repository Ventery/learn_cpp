#include<iostream>
using namespace std;
class B
 
{
 
public:
 
    int ib;
 
public:
 
    B(int i=1) :ib(i){}
 
    virtual void f() { cout << "B::f()" << endl; }
 
    virtual void Bf() { cout << "B::Bf()" << endl; }
 
};
class B1 :  virtual public B
 
{
 
public:
 
    int ib1;
 
public:
 
    B1(int i = 100 ) :ib1(i) {}
 
    virtual void f() { cout << "B1::f()" << endl; }
 
    virtual void f1() { cout << "B1::f1()" << endl; }
 
    virtual void Bf1() { cout << "B1::Bf1()" << endl; }
 
};
int main()
{
	B p0(123);
	B1 p(9527);
	return 0;
}