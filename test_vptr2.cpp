#include<iostream>
using namespace std;
class Base
{
public:
	Base(long i) : baseI(i) {cout << "Base::Base()" << endl;};
	long getI() { cout << "Base::getI()" << endl; return baseI; }
	static void countI() {cout << "Base::countI()" << endl;};
	virtual ~Base() {cout << "Base::~Base()" << endl;}
	virtual void print(void) { cout << "Base::print()" << endl; }
	virtual void read(void) { cout << "Base::read()" << endl; }
	virtual void ini(void) { cout << "Base::ini()" << endl; }

private:
	long baseI;
	static long baseS;
};
void testBase( Base&p)
{
	cout << "对象的内存起始地址：" << &p << endl;
	//验证虚表
	cout<<"虚函数表地址："<<(long *)*(long *)(&p)<<endl;
	cout << "虚函数表第一个函数的地址：" << (long *)*(long *)*(long *)(&p) << endl;
	cout << "虚函数表中，第二个虚函数即print（）的地址：" << (long *)*((long *)*(long *)(&p)+1) << endl;
	cout << "类名称："<<(char *)*((long *)*(long *)(&p)+6)<<endl;

	//通过地址调用虚函数print（）
	typedef void(*Fun)(void);
	//Fun Isdec = (Fun)*(long *)*(long *)(&p);
	//Isdec();
	Fun IsPrint=(Fun)*((long *)*(long *)(&p));
	IsPrint();

	/*cout << endl;
	cout << "调用了虚函数print";
	IsPrint(); //若地址正确，则调用了Base类的虚函数print（）
	cout << endl;

	//输入static函数的地址
	p.countI();//先调用函数以产生一个实例
	cout << "static函数countI()的地址：" << p.countI << endl;

	//验证nonstatic数据成员
	cout << "推测nonstatic数据成员baseI的地址：" << (long *)(&p) + 1 << endl;
	cout << "根据推测出的地址，输出该地址的值：" << *((long *)(&p) + 1) << endl;
	cout << "Base::getI():" << p.getI() << endl;*/

}
int main()
{
	Base b(1000);
	testBase(b);
	return 0;
}