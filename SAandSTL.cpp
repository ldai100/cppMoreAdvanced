#include <iostream>
#include <cstdlib>
#include <cassert>
#include <algorithm>

using namespace std;

template <typename T>
class SA{

private:
	int low, high;
	T* p;

public:

	//default constructor
	//allows for writing things like SA a;

	SA(){low=0; high=-1;p=NULL;}

	//2 parameter constructor lets us write
	//SA x(10,20);

	SA(int l, int h){
		if((h-l+1) <= 0){
			cout<<"constructor error in bounds definition"<<endl;		
			exit(1);
		}
		low=l;
		high=h;
		p=new T[h-l+1];
	}

	//single parameter constructor lets us
	//create a SA almost like a "standard" one by writing
	//SA x(10); and getting an array x index from 0 to 9

	SA(int i){low=0; high=i-1;
	p=new T[i];
	}

	//copy constructor for pass by value and
	//initialization

	SA(const SA& s){
		int size=s.high-s.low+1;
		p=new T[size];
		for(int i=0; i<size; i++)
			p[i]=s.p[i];
		low=s.low;
		high=s.high;
	}

	//destructor

	~SA(){
		delete[] p;
	}

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
		delete[] p;
		int size=s.high-s.low+1;
		p=new T[size];
		for(int i=0; i<size; i++)
			p[i]=s.p[i];
		low=s.low;
		high=s.high;
		return *this;
	}

	//overloads << so we can directly print SAs
	
	template <typename U>
	friend ostream& operator<<(ostream& os, SA<U> s);

//-----------------------------------------------------------
//due to simplicity of SA class, I don't even need a iterator;
//I only need two pointers, begin and end;

T* begin(){
	return p+0; //0 is always the beginning of the array;
}

T* end(){
	return p+(high-low+1);
	//the +1 is because we have to go 1 spot after the
	//last index to iterator through the whole list;
}


struct iterator{
	friend class SA;
	T* pos;
	
	iterator(T* init = 0):pos(init){}

	T& operator*(){
		return *pos;
	}

	const T& operator*() const{
		return *pos;
	}

	iterator& operator++(){
		pos++;
		return pos;
	}

	iterator operator++(int){
		T* temp = pos;
		pos++;
		return temp;
	}

	bool operator==(const T* x) const{
		return pos == x;
	}

	bool operator!=(const T* x) const{
		return pos != x;
	}

};


//note not even ++ need to be overloaded because build-in ++
//for pointer to T* would work;

//-----------------------------------------------------------

};

template <typename U>
ostream& operator<<(ostream& os, SA<U> s){
	int size=s.high-s.low+1;
	for(int i=0; i<size; i++)
		os<<s.p[i]<<" ";
	return os;
}


//-----------------------------------------------------------
//Everything in main is for testing and thus new;

int main(){
	SA<int> a(20);
	SA<int> b(10,29);
	int start=0;
	//initializing the arrays;
	for(int i=19; i>=0; i--){
		a[i]=start;
		b[i+10]=start+10;
		start++;
	}
	cout<<endl;
	cout<<"SA<int> a created and it's content is:"<<endl;
	cout<<a<<endl<<endl;
	cout<<"SA<int> b created and it's content is:"<<endl;
	cout<<b<<endl<<endl;
	cout<<"Testing find in a SA<int>:"<<endl;
	int* address;
	for(int i=10; i<25; i++){
		address=find(a.begin(), a.end(), i);
		if(address != a.end()) cout<<i<<" Found!"<<endl;
		else cout<<i<<" Not found!"<<endl;
	}
	cout<<endl<<endl;

	cout<<"Testing find in b SA<int>:"<<endl;
	for(int i=20; i<40; i++){
		address=find(b.begin(), b.end(), i);
		if(address != b.end()) cout<<i<<" Found!"<<endl;
		else cout<<i<<" Not found!"<<endl;
	}
	cout<<endl<<endl;

	cout<<"Testing sort with a, output:"<<endl;
	sort(a.begin(), a.end());
	cout<<a<<endl<<endl;
	
	cout<<"Testing sort with b, output:"<<endl;
	sort(b.begin(), b.end());
	cout<<b<<endl<<endl;
	

	return 0;
}
