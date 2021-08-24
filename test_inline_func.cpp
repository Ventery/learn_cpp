#include<iostream>

using namespace std;

class B;//先申明后定义

class A {  

public:  

    A()   {    B*pb = new B;    cout<<"A()";   };

  ~A()   {    cout<<"~A()";   }

};

class B

{

public:  

B()  {   cout<<"B()";  };

~B()  {   cout<<"~B()";  };

};

int main()

{

    A test;  

    cout<<"Hello C++ world!"<<endl;  return 0;

}
