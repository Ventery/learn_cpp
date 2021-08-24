/*
总结就是，类的
静态函数成员可以直接取&Base::test_inner_static_func
虚函数成员需要查虚函数表，不必传入对象地址，此时如果虚函数访问了类的普通成员变量会乱码（原因以后研究）
普通成员见下，调用必须传入对象地址
*/

#include<iostream>
using namespace std;
class Base
{
public:
	Base() {cout << "Default constructor!" << endl;}

	Base(int i) : baseI(i) {}

	virtual void print(void) { cout << "this=" << this << " 调用了虚函数Base::print() baseI:" << baseI << endl; }

	virtual void setI() {cout << "调用了虚函数Base::setI()" << endl;}

	static void test_inner_static_func() {cout << "调用了静态函数Base::test_inner_static_func()" << endl;}

	virtual ~Base() {cout << this << "析构了" << endl;}
	void normal_fun() { cout << "this=" << this << " 调用了普通函数Base::normal_fun() baseI:" << baseI << endl;  }

private:

	int baseI;

};
void test()
{
}
int main()
{
	Base a(1), b(2), c(3);
	cout << "a的地址为:" << &a << endl;
	cout << "b的地址为:" << &b << endl;
	cout << "c的地址为:" << &c << endl;

	cout << (long *)*(long *)(&a) << " " << (long *)*(long *)(&b) << endl;
	/*测试直接调用虚函数*/
	typedef void( *Fun)(void);
	union {
		void *pv;
		void(Base::*pfn)();
	} inner_vfunc;
	Fun vfunc = (Fun) * ( (long *) * (long*)(&b));

	inner_vfunc.pfn = &Base::print;

	cout << "vfunc :" << (long *)vfunc << endl;
	cout << "inner_vfunc :" << (long *)(inner_vfunc.pv) << endl;

	vfunc();
	vfunc();
	vfunc();
	b.print();

	/*测试调用类的普通函数*/
	inner_vfunc.pfn = &Base::normal_fun;

	cout << "inner_vfunc(普通成员函数) :" << (long *)(inner_vfunc.pv) << endl;

	(b.*inner_vfunc.pfn)();
	//vfunc=&Base::normal_fun; //don't work
	//vfunc();   //don't work
	return 0;
}
