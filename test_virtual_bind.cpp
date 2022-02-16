#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;
struct Base
{
    virtual void foo() { cout << "Base\n"; }
};

struct Derived : public Base
{
    virtual void foo() { cout << "Derived\n"; }
};

int main(int argc, const char * argv[])
{
    Base* base = new Derived;
    auto baseMethodHopefully = std::bind( &Base::foo, base );
    baseMethodHopefully();    // Want call to Base::foo(), but get call to Derived::foo().

    return 0;
}
