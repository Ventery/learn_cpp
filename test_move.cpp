#include<iostream>
#include<stdio.h>
using namespace std;
class tba
{
	public:
	tba(){printf("cons\n");};
	private:
	tba(const tba &){printf("&\n");};
	public:
	tba(const tba &&){printf("&&\n");};
};
void fun1(tba &a)
{
	printf("fun2\n");
}

void fun1(tba &&a)
{
	printf("fun3\n");
}
int main()
{
	printf("e1\n");
	tba e1;
	printf("e2\n");
	tba &&e2=std::move(e1);
	printf("e3\n");
	tba &&e3=tba();
	printf("e4\n");
	tba e4=tba();
	printf("e5\n");
	tba e5(std::move(tba()));
	printf("e6\n");
	tba e6=std::move(tba());

	printf("test func\n");

	fun1(tba());

	return 0;
}
