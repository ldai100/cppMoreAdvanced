#include <iostream>
#include <cstdlib>
#include <cassert>
#include <initializer_list>


using namespace std;

//--------------------------------------------------------
//smart pointer class;

template <typename T>
class smartPointer{
private:
	T* pointee;  //raw pointer;

public:
	smartPointer(T* raw = 0):pointee(raw){}

	smartPointer(smartPointer<T>& rhs){
		pointee = rhs.pointee;
		rhs.pointee = 0;
	}	


	//destructor;
	~smartPointer(){
		if(pointee != 0) delete [] pointee;
	}

        smartPointer<T>& operator=(smartPointer<T>& rhs){
		if(this == &rhs) return *this;
		delete pointee;

		pointee = rhs.pointee;
		rhs.pointee = 0;
		return *this;
	}


	T* operator->() const{
		return pointee;
	}

	T& operator*() const{
		return *pointee;
	}

	T& operator[](int i) const{
		return pointee[i];
	}


};

//smart pointer class;
//--------------------------------------------------------

template <typename T>
class SA{

private:
	int low, high;
	smartPointer<T> p;

public:
	//default constructor
	//allows for writing things like SA a;

	SA(){low=0; high=-1;p=smartPointer<T>(new T[0]);}

	//2 parameter constructor lets us write
	//SA x(10,20);

	SA(int l, int h){
		if((h-l+1) <= 0){
			cout<<"constructor error in bounds definition"<<endl;		
			exit(1);
		}
		low=l;
		high=h;
		p=smartPointer<T>(new T[h-l+1]);
	}



//----------------------------------------------------------------------------
//new code added below!!!!!!!!!
	
	//initializer list constructor;

	SA(initializer_list<T> l){
		int size = l.size();
		low=0; high=size-1;
		p=smartPointer<T>(new T[size]);
		initializer_list<int>::iterator itr;
		itr=l.begin();
		for (int i=0; i<size; i++){
		    p[i]= *itr;
		    itr++;
		}
	}


//----------------------------------------------------------------------------
//new code above!!!!!!!!!

	//single parameter constructor lets us
	//create a SA almost like a "standard" one by writing
	//SA x(10); and getting an array x index from 0 to 9

	SA(int i){low=0; high=i-1;
	p=smartPointer<T>(new T[i]);
	}

	//copy constructor for pass by value and
	//initialization

	SA(const SA& s){
		int size=s.high-s.low+1;
		p=smartPointer<T>(new T[size]);
		for(int i=0; i<size; i++){
			p[i]=s.p[i];
		}
		low=s.low;
		high=s.high;
	}

	//destructor

	~SA(){}

	int getHigh(){
		return high;
	}

	int getLow(){
		return low;
	}

	//overloaded[] lets us write
	//SA x(10,20); x[15]=100;

	T& operator[](int i){
		if(i<low || i>high){
			cout<<"index "<<i<<" out of range"<<endl;
			exit(1);
		}
		return p[i-low];
	}

	//overloaded assignment lets us assign
	//one SA to another

	SA& operator=(const SA& s){
		if(this==&s) return *this;
		int size=s.high-s.low+1;
		p=smartPointer<T>(new T[size]);
		for(int i=0; i<size; i++)
			p[i]=s.p[i];
		low=s.low;
		high=s.high;
		return *this;
	}

	//overloads << so we can directly print SAs
	
	friend ostream& operator<<(ostream& os, const SA<T>& s){
		int size=s.high-s.low+1;
		for(int i=0; i<size; i++)
			os<<s.p[i]<<" ";
		return os;
	}

};



int main(){
	SA<int> z{10,20,30};
	cout<<"printing result for SA z {10,20,30};"<<endl;

	cout<<z<<endl;
	cout<<"SA z(1,3){10,20,30}; would not work because" <<endl;
	cout<<"both (1,3) {10,20,30} are parameters where the latter"<<endl;
	cout<<"one is a initializer_list. In addition, {} are used for"<<endl;
	cout<<" initialization not assignment. But this would work:"<<endl;
	cout<<"SA<int> x({5,6,7});"<<endl;

	SA<int> x({5,6,7});
	cout<<x<<endl;

//	SA<int> v(1,3){1,2,3};
	return 0;
}
