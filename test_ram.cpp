#include <iostream>

using namespace std;

int main()
{
 int j=0;
 int a[3] = {0},b[3]={0};
 int i=0;
 for (; i <= 3; i++)
 {
  a[i] = 0;
  printf("hello world \n");
  cout<<a+i<<" "<<b+i<<endl;
 }
 cout<<&i<<" "<<&j<<endl;
 return 0;
}
