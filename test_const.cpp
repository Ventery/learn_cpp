class Noncopyable
{
protected:
  Noncopyable() {}
  ~Noncopyable() {}

private:
  Noncopyable(const Noncopyable &);
  const Noncopyable &operator=(const Noncopyable &);
};

template <class T>
class Singleton : Noncopyable
{
public:
    Singleton(){};
    ~Singleton(){};
    static const T &GetInstance() 
    { 
        if (instance_ == nullptr)
        {
            instance_ = new T();
        }
        return *instance_; 
    }

protected:
    static const T *instance_;
};

template<typename T>
const T * Singleton<T> :: instance_ = nullptr;

class Test : public Singleton<Test>{
public :
	int a;
};


#include <iostream>
using namespace std;
int main(){
	cout<<Test::GetInstance().a<<endl;
}