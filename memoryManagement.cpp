#include <iostream>
#include <cstdlib>
#include <cassert>

using namespace std;


template <typename T>
class SA{

private:
	int low, high, length;
//-------------------------------------------------------------------------
//variable changes made here;
	static SA<T> *newlist;
	union{
		SA<T> *freepointer;
		T* p;		
	};

public:


//--------------------------------------------------------------------------
//overloading operator new and delete;
//not sure what professor wanted, but they are overloaded and class specific now....

void* operator new(size_t size){
	if(size != sizeof(SA<T>)){
		return malloc(size);
	}
	else if(!newlist){
		newlist = (SA<T> *)new T[100*sizeof(SA<T>)];
		int temp;
		for(int i=0; i<99; i++){
			newlist[i].freepointer = &(newlist[i+1]);
			temp = i;
		}
		newlist[temp].freepointer=0;
	}

	
	SA<T> *savenew = newlist;
	newlist = newlist->freepointer;
	return savenew;
}

void operator delete(void* ptr){
	SA<T> *s = (SA *)ptr;
	s->freepointer = newlist;
	newlist = s;
}


//--------------------------------------------------------------------------

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

};

//-------------------------------------------------------------
//initialize static member;
template<typename T>
SA<T>* SA<T>::newlist=0;

//-------------------------------------------------------------
template <typename U>
ostream& operator<<(ostream& os, SA<U> s){
	int size=s.high-s.low+1;
	for(int i=0; i<size; i++)
		os<<s.p[i]<<" ";
	return os;
}

template <class T>
class safeMatrix{

private:
	int rowSize, low, high;
	SA < SA<T> > *m;

public:
	
	//constructor to create row x col matrix;
	safeMatrix(int r, int c){
		low=0;
		high=r-1;
		rowSize=r;
		m= new SA< SA<T> >(0, r-1);
		for(int i=0; i<r; i++){
			(*m)[i]= *(new SA<T>(0,c-1));
		}	
	}

	//constructor to create specified bounds;
	safeMatrix(int rowl, int rowh, int coll, int colh){
		low=rowl;
		high=rowh;
		rowSize=rowh-rowl+1;
		m= new SA< SA<T> >(rowl, rowh);
		for(int i=rowl; i<=rowh; i++){
			(*m)[i]= *(new SA<T>(coll, colh));
		}
	}

	
	//overload [];
	SA<T>& operator[](int i){
		return (*m)[i];
	}

	//overload *;
	//requires to return a new matrix, no modifications made;
	safeMatrix<T> operator*(safeMatrix<T> s){
		int row1, row2, col1, col2;
		row1 = high - low + 1;
		col1 = m[low].getHigh() - m[low].getLow() + 1;
		row2 = s.getHigh() - s.getLow() + 1;
		col2 = s.getSA()[s.getLow()].getHigh() - s.getSA()[s.getLow()].getLow() + 1;

		if(col1 != row2){
			cout<<"incompatible matrices!";
			exit(1);
		}
		
		//product of matrices should have the size
		//row1 x col2;
		safeMatrix<T> result(row1, col2);
		//zeroize result;
		for(int i=0; i<row1; i++){
			for(int j=0; j<col2; j++){
				result[i][j]=0;
			}
		}		

		int col1Low, col2Low, row2Low;;
		col1Low = m[low].getLow();
		col2Low = s.getSA()[s.getLow()].getLow();		
		row2Low = s.getLow();
		for(int i=0; i<row1; i++){
			for(int j=0; j<col2; j++){
				for(int k=0; k<col1; k++){
					result[i][j] += (*m)[i+low][k+col1Low] * s[k+row2Low][j+col2Low];
				}
			}
		}
		return result;
	}

	template <typename K>
	friend ostream& operator<<(ostream& os, safeMatrix<K> sm);

	SA < SA<T> > getSA() const{
		return *m;
	}

	int getSize() const{
		return rowSize;
	}

	int getHigh() const{
		return high;
	}

	int getLow() const{
		return low;
	}
};

template <typename K>
ostream& operator <<(ostream& os, safeMatrix<K> sm){
	for(int i=sm.getLow(); i<sm.getSize()+sm.getLow(); i++)
		os<<(*sm.m)[i]<<endl;
	return os;
}


int main(){
	safeMatrix<int> a(5,5);
	safeMatrix<int> b(5,9,5,9);
	safeMatrix<int> c(5,5);
	//initialize both matrices;
	for(int i=0; i<5; i++){
		for(int j=0; j<5; j++){
			a[i][j]=i;
			c[i][j]=j;

		}
	}	
	for(int i=5; i<=9; i++){
		for(int j=5; j<=9; j++){
			b[i][j]=i;
		}
	}

	cout<<"testing << with a:"<<endl;
	cout<<a;
	cout<<"testing << with b:"<<endl;
	cout<<b;

	cout<<"testing << with c:"<<endl;
	cout<<c;
	
	//requirement asks to return a new matrix;
	cout<<"testing * with 0-N range by a*c:"<<endl;
	cout<<a*c;

	cout<<"testing * with N-M range by a*b:"<<endl;
	cout<<a*b;


	return 0;
}
