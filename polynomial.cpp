#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;


template<class T>
class node{
	private:
		T coef;
		T exp;
		node* Link;
	public:
		node(){Link=0;}
		node(T c, T e){coef=c; exp=e; Link=0;}
		T& getCoef(){return coef;}
		T& getExp(){return exp;}
		node* & link(){return Link;}
};

template<class T>
void print_list(node<T>* p, ostream& out){
	node<T>* first = p;
	bool firstTerm=true;
	while(p){
		//coefficient part:
		T c = p->getCoef();

		if(c == 0){ p=p->link(); continue;}//if coef is 0, skip;
		else if(c == 1 && firstTerm){cout<<""; out<<""; firstTerm=false;} //print no coef;
		else if(c == 1 && !firstTerm){cout<<"+"; out<<"+";}
		else if(c>0 && !firstTerm) {cout<<"+"<<c; out<<"+"<<c;}
		else {cout<<c; out<<c; firstTerm=false;}   //all other conditions simply print c;
		
		//exponent part; 
		T e = p->getExp();

		if(e == 0){ p=p->link(); continue;}    //skip since it's 1;
		else if(e == 1){ cout<<"x";out<<"x";} //print only x;
		else {cout<<"x^"<<e; out<<"x^"<<e;}

		p=p->link();
	}
	cout<<endl;
	out<<endl;
}


template<class T>
node<T>* make_int_list(stringstream& ss){
	node<T>* first, *p, *q;
	T c, e;
	ss>>c>>e;
	first=p=new node<int>(c,e);
	while(!ss.eof()){
		ss>>c>>e;
		q=new node<int>(c,e);
		p->link()=q;
		p=q;
	}
	return first;
}



template<class T>
node<T>* product(node<T>* x, node<T>* y){
	node<T>* first, *p, *q;
	int count=0;//need it to initialize the first node;
	while(x){

		node<T>* ys=y; //back to the beginning of the list;
		while(ys){
			T xcoef,xexp,ycoef,yexp;
			xcoef=x->getCoef();
			xexp=x->getExp();
			ycoef=ys->getCoef();
			yexp=ys->getExp();

			if(count==0){
				first=p=new node<T>(xcoef*ycoef, xexp+yexp);
				count++;
				ys=ys->link();
				continue;
			}
			else{
				q=new node<T>(xcoef*ycoef, xexp+yexp);
			}
								
			p->link()=q;
			p=q;
			count++;
			
			ys=ys->link();
		}
		x=x->link();
	}
	manageList(first);
	sort_list(first);
	return first;
}


//the design of this sum function will only work if the polynomial are sorted;
template<class T>
node<T>* sum(node<T>* x, node<T>* y){
	node<T>* first=x;
	node<T>* second=y;
	node<T>* head, *p, *q;
	int coef, exp;
	head=p=new node<T>(0,0);
	//loops through both list until at least one of them is at the end;
	while(x && y){
		if(x->getExp() > y->getExp()){
			exp = x->getExp();
			coef= x->getCoef();
			x=x->link();
		}

		else if(x->getExp() < y->getExp()){
			exp = y->getExp();
			coef= y->getCoef();
			y=y->link();
		}
		else{
			exp = x->getExp();
			coef= x->getCoef()+y->getCoef();
			x=x->link();
			y=y->link();
		}
		q=new node<T>(coef, exp);
		p->link()=q;
		p=q;
	}

	//loops through the rest of the available list;
	while(x || y){
		if(x){
			exp=x->getExp();
			coef=x->getCoef();	
			x=x->link();
		}
		if(y){
			exp=y->getExp();
			coef=y->getCoef();
			y=y->link();
		}	
		q=new node<T>(coef, exp);
		p->link()=q;
		p=q;
	}
	return head;
}

//the design of this subtract function will only work if the polynomials are sorted;
template<class T>
node<T>* difference(node<T>* x, node<T>* y){
	node<T>* first=x;
	node<T>* second=y;
	node<T>* head, *p, *q;
	int coef, exp;
	head=p=new node<T>(0,0);
	//loops through both list until at least one of them is at the end;
	while(x && y){
		if(x->getExp() > y->getExp()){
			exp = x->getExp();
			coef= x->getCoef();
			x=x->link();
		}

		else if(x->getExp() < y->getExp()){
			exp = y->getExp();
			coef= y->getCoef();
			y=y->link();
		}
		else{
			exp = x->getExp();
			coef= x->getCoef()-y->getCoef();
			x=x->link();
			y=y->link();
		}
		q=new node<T>(coef, exp);
		p->link()=q;
		p=q;
	}

	//loops through the rest of the available list;
	while(x || y){
		if(x){
			exp=x->getExp();
			coef=x->getCoef();	
			x=x->link();
		}
		if(y){
			exp=y->getExp();
			coef=y->getCoef();
			y=y->link();
		}	
		q=new node<T>(coef, exp);
		p->link()=q;
		p=q;
	}
	return head;
}



//sort function used to form canonical form;

template<class T>
void sort_list(node<T>* first){
	node<T>* begin=first;
	node<T>* traverse;
	node<T>* max;

	manageList(first);
	while(begin->link()){
		max=begin;
		traverse=begin->link();

		while(traverse){
			//find min from the list;
			if(max->getExp() < traverse->getExp()){
				max=traverse;
			}
			traverse=traverse->link();
		}
		swap(begin,max);
		begin=begin->link();
	}
}

//this function is to combine like terms;
template<class T>
void manageList(node<T>* first){
	node<T>* current=first;
	node<T>* traverse;
	while(current){
		traverse=current->link();
		while(traverse){
			if(current->getExp()==traverse->getExp()){
				//add the coefficients and set the one added to 0s;
				current->getCoef() += traverse->getCoef();
				traverse->getCoef()=0;
				traverse->getExp()=0;
			}
			traverse=traverse->link();
		}
		current=current->link();
	}	
}


template<class T>
void swap(node<T>* p, node<T>* q){
	//I'm not swapping around nodes, but rather
	//change the values because it's easier
	//this way for a singly linked list;
	T tempCoef, tempExp;
	tempCoef=p->getCoef();
	tempExp=p->getExp();
	p->getCoef()=q->getCoef();
	p->getExp()=q->getExp();
	q->getCoef()=tempCoef;
	q->getExp()=tempExp;
}



template<class T>
void add_at_end(node<T>* &first, node<T>* r){
	node<T>* p;
	p=first;
	while(p->link()){
		p=p->link();
	}
	p->link()=r;
}


int main(int argc, char *argv[]){

	// in this program, if the input file is
	// missing a integer, then it is assume to be 1;	
	const char* filename="input.txt";
	const char* outfile="output.txt";
	node<int>* first;
	node<int>* second;

	ifstream infile(filename);
	ofstream out(outfile);
	out<<"If odd number of integers are given, the last exponent is assumed to be 1.";
	out<<endl;
	int i;
	string s;
	getline(infile, s);  //get first line to s;
	stringstream ss1(s);
	first=make_int_list<int>(ss1);

	getline(infile, s);
	stringstream ss2(s);
	second=make_int_list<int>(ss2);

	out<<"Original input:"<<endl;
	print_list(first, out);
	print_list(second, out);

	out<<"Canonical form:"<<endl;
	sort_list(first);
	sort_list(second);
	print_list(first, out);
	print_list(second, out);

	out<<"Sum:"<<endl;
	print_list(sum(first,second), out);

	out<<"Difference:"<<endl;
	print_list(difference(first,second), out);

	out<<"Product:"<<endl;
	print_list(product(first,second), out);


	infile.close();
	out.close();

	return 0;
}
