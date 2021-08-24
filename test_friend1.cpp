#include<iostream>
#include"noncopyable.h"
class C_test:noncopyable
{
friend int main();
};
int main()
{
	C_test c_1;
	C_test &c_2=c_1;
	return 0;
}
