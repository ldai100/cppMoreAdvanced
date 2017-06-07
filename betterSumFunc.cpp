#include <iostream>

using namespace std;

template <typename T>
T sum(T t)
{
  return t;
}

template <typename T, typename... Parameters>
T sum(T first, Parameters... parameters)
{
  return first + sum(parameters...);
}


int main(){
  cout << sum(1,2,3) << endl;
  cout << sum(10,20) << endl;
  cout << sum(1,1,1,1,1,1,1,1,1) << endl;
  return 0;
}
