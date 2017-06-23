#include <iostream>
#include <cstdlib>
#include <limits.h>
#include <cassert>

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

};

template <typename U>
ostream& operator<<(ostream& os, SA<U> s){
	int size=s.high-s.low+1;
	for(int i=0; i<size; i++)
		os<<s.p[i]<<" ";
	return os;
}

//matrix class;
//*****************************************************************************


template <class T>
class safeMatrix{

private:
	int rowSize, colSize, low, high;
	SA <T> m;

public:
	
	//constructor to create row x col matrix;
	safeMatrix(int r, int c){
		low=0;
		high=r-1;
		rowSize=r;
		colSize=c;
		m=SA<T> (0, r*c-1);
	}

	//constructor to create specified bounds;
	safeMatrix(int rowl, int rowh, int coll, int colh){
		low=rowl;
		high=rowh;
		rowSize=rowh-rowl+1;
		colSize=colh-coll+1;
		m=SA<T> (rowh-rowl, colh-coll);
	}
	
	//overload [];
	T& operator[](int i){
		return m[i];
	}

//multiplication is not needed in this project;
/*
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
					result[i][j] += m[i+low][k+col1Low] * s[k+row2Low][j+col2Low];
				}
			}
		}
		return result;
	}
*/

	friend ostream& operator <<(ostream& os, safeMatrix<T> sm){
		int count=1;
		int r=sm.getSize();
		int c=sm.getCSize();
		for(int i=0; i<r*c; i++){
			os<<sm.m[i]<<" ";
			if(count%c==0) os<<endl;
			count++;
		}
		return os;
	}


	SA<T> getSA() const{
		return m;
	}

	int getSize() const{
		return rowSize;
	}

	int getCSize() const{
		return colSize;
	}

	int getHigh() const{
		return high;
	}

	int getLow() const{
		return low;
	}
};


//*******************************************************************
//new codes are here and below;


class VNT{
private:
	int row, col;
	safeMatrix<int>* v;

public:

	//required constructor;
	VNT(int m, int n){
		row = m;
		col = n;
		v = new safeMatrix<int> (row, col);

		//initialize this empty VNT;
		for(int i=0; i<row*col; i++){
			(*v)[i]=INT_MAX;
		}
	}


	//functions;

	//magic happens here, :) just kidding;
	//insertion sort implemented;

	void sort(int *k, int size){
		for(int i=0; i<size-1;i++){
			for(int j=i+1; j<size;j++){
				if(k[j]<k[i]){
					int temp=k[i];
					k[i]=k[j];
					k[j]=temp;
				}	
			}
		}
	}


	//simply put in to a spot that's not taken;
	void add(int n){
		if((*v)[row*col-1]!=INT_MAX){
			cout<<"VNT full!"<<endl;
			return;
		}
		for(int i=0; i<row*col; i++){
			if((*v)[i]==INT_MAX){
				(*v)[i]=n;
				break;
			}
		}
		//pass the address of the first element;
		sort(&(*v)[0], row*col);
	}
	
	int getMin(){
		int min = (*v)[0];
		(*v)[0]=INT_MAX;
		sort(&(*v)[0], row*col);
		return min;
	}

	bool find(int target){
		for(int i=0; i<row*col; i++){
			if((*v)[i]==target) return true;
		}
		return false;
	}

	friend ostream& operator <<(ostream& os, VNT vnt){
		os<<*(vnt.v);
		return os;
	}



};




int main(){
	VNT a(5,7);
	if(a.find(10))
		cout<<"Find the number 16 in the VNT"<<endl<<endl;
	else
		cout<<"Cannot find the number 16 in the VNT"<<endl<<endl;
	cout<<"Using add function:"<<endl;
	a.add(14);
	cout<<a<<endl;
	a.add(7);
	cout<<a<<endl;
	a.add(11);
	cout<<a<<endl;
	a.add(5);
	cout<<a<<endl;
	a.add(9);
	cout<<a<<endl;
	a.add(6);
	cout<<a<<endl;
	a.add(8);
	cout<<a<<endl;
	a.add(19);
	cout<<a<<endl;
	a.add(16);
	cout<<a<<endl;
	if(a.find(16))
		cout<<"Find the number 16 in the VNT"<<endl<<endl;
	else
		cout<<"Cannot find the number 16 in the VNT"<<endl<<endl;
		
	cout<<"++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl<<endl;

	cout<<"Using getMin() function:"<<endl;
	for(int i=0; i<9; i++) {
		cout<<"NO."<<(i+1)<<" : "<<a.getMin()<<endl<<a<<endl;
	}
	int w[7]= {4,7,2,32,75,21,6};
	cout<<"Sort w[7]={4,7,2,32,75,21,6}:"<<endl;
	a.sort(w,7);
	for(int i=0; i<7; i++) {
		cout<<w[i]<<" ";
	}
	cout<<endl;

	return 0;
}
