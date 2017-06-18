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
void print_list(node<T>* p){
	node<T>* first = p;
	while(p){

		//coefficient part:
		T c = p->getCoef();

		if(c == 0) continue;	//if coef is 0, skip;
		else if(c == 1){cout<<"";} //print no coef;
		else if(c>0 && p!=first) {cout<<"+"<<c;}
		else {cout<<c;}	   //all other conditions simply print c;
		
		//exponent part; 
		T e = p->getExp();

		if(e == 0) continue;    //skip since it's 1;
		else if(e == 1){ cout<<"x";} //print only x;
		else {cout<<"x^"<<e;}

		p=p->link();
	}
	cout<<endl;
}

node<int>* make_int_list(stringstream& ss){
	node<int>* first, *p, *q;
	int c, e;
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

//sort function used to form canonical form;

template<class T>
void sort_list(node<T>* first){
	node<T>* begin=first;
	node<T>* traverse;
	node<T>* max;

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
	
	const char* filename="input.txt";
	node<int>* first;
	node<int>* second;

	ifstream infile(filename);
	int i;
	string s;
	getline(infile, s);  //get first line to s;
	stringstream ss1(s);
	first=make_int_list(ss1);

	getline(infile, s);
	stringstream ss2(s);
	second=make_int_list(ss2);

	print_list(first);
	print_list(second);



	system("PAUSE");
	return EXIT_SUCCESS;
}
