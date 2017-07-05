#include <iostream>
using namespace std;
class z{
public:
z(){cout<<"enter"<<endl;}
~z(){cout<<"exit"<<endl;}
};
class X
{
 int i;
 int j;
public:
 void destroy();
 ~X() {z a; cout << "X's dtor\n"; } // POINT 1
};
void X::destroy() {z a; delete this; }
class Y : public X
{
 int *j;
public:
Y(){ j= new int;}
~Y() {z a; cout << "Y's dtor\n"; delete j; }
};
int main()
{
 X* x = new Y;
 cout<<"going to destroy"<<endl;
 x->destroy();
 cout<< "from main: Do we call destructors now?" <<endl;
}
