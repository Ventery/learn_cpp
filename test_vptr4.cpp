#include<iostream>
using namespace std;
class Base
{
public:

	Base(int i) : baseI(i) {};
	virtual ~Base() {}

	int getI() { return baseI; }

	static void countI() {};

	virtual void print(void) { cout << "Base::print()"<<endl; }

private:

	int baseI;

	static int baseS;
};
class Base_2
{
public:
	Base_2(int i) : base2I(i) {};

	virtual ~Base_2() {}

	int getI() { return base2I; }

	static void countI() {};

	virtual void print(void) { cout << "Base_2::print()"<<endl; }

private:

	int base2I;

	static int base2S;
};

class Base_3
{
public:
	Base_3(int i) : base3I(i) {};

	virtual ~Base_3() {}

	int getI() { return base3I; }

	static void countI() {};

	virtual void print(void) { cout << "Base_3::print()"<<endl; }

private:

	int base3I;

	static int base3S;
};

class Drive_multyBase : public Base, public Base_2, public Base_3
{
public:

	Drive_multyBase(int d) : Base(1000), Base_2(2000) , Base_3(4000), Drive_multyBaseI(d) {};

	virtual void print(void) { cout << "Drive_multyBase::print" <<endl; }

	virtual void Drive_print() { cout << "Drive_multyBase::Drive_print" <<endl; }

private:
	int Drive_multyBaseI;
};
int main()
{
	Drive_multyBase p(7777);
	Base *bp;
	Base_2 *bp2;

	return 0;
}