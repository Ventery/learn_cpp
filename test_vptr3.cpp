#include<iostream>
using namespace std;
class Base
{
public:
	Base(int i) : baseI(i) {};
	int getI() { return baseI; }
	static void countI() {};
	virtual ~Base() {}
	virtual void print(void) { cout << "Base::print()"; }

private:

	int baseI;

	static int baseS;
};
class Derive : public Base
{
public:
	Derive(int d) : Base(1000),      DeriveI(d) {};
	//overwrite父类虚函数
	virtual void Drive_print() { cout << "Drive::Drive_print()" ; }
	virtual void print(void) { cout << "Drive::Drive_print()" ; }
	// Derive声明的新的虚函数
	virtual ~Derive() {}
private:
	int DeriveI;
};
int main()
{
	Derive b(4396);
	return 0;
}