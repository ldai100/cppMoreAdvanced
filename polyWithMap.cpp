#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <algorithm>

using namespace std;


template<class T>
void product(map<T, T, greater<int> >& poly1, map<T, T, greater<int> >& poly2, map<T,T,greater<int> >& result){

	auto i = poly1.begin();
	auto j = poly2.begin();

	while(i != poly1.end()){

		j=poly2.begin();
		while(j != poly2.end()){
			result[(i->first)+(j->first)] += (i->second)*(j->second);
			j++;
		}
		i++;
	}
}

template<class T>
void sum(map<T, T, greater<int> >& poly1, map<T, T, greater<int> >& poly2, map<T,T, greater<int> >& result){
	auto i = poly1.begin();
	auto j = poly2.begin();

	int coef, exp;
	while(i != poly1.end() && j != poly2.end()){
		if((exp=i->first) > (j->first)){
			coef=i->second;		
			i++;
		}
		else if((i->first) < (exp=j-> first)){
			coef=j->second;
			j++;
		}
		else{
			exp = i->first;
			coef= i->second + j->second;
			i++;
			j++;
		}
		result[exp]=coef;
	}

	while(i != poly1.end() || j != poly2.end()){ 
		if(i != poly1.end()){
			exp=i->first;
			coef=i->second;
			i++;
		}
		if(j != poly2.end()){
			exp=j->first;
			coef=j->second;
			j++;
		}
		result[exp]=coef;
	}
}

template<class T>
void difference(map<T, T, greater<int> >& poly1, map<T, T, greater<int> >& poly2, map<T,T, greater<int> >& result){
	auto i = poly1.begin();
	auto j = poly2.begin();

	int coef, exp;
	while(i != poly1.end() && j != poly2.end()){
		if((exp=i->first) > (j->first)){
			coef=i->second;		
			i++;
		}
		else if((i->first) < (exp=j-> first)){
			coef=-(j->second);
			j++;
		}
		else{
			exp = i->first;
			coef= i->second - j->second;
			i++;
			j++;
		}
		result[exp]=coef;
	}

	while(i != poly1.end() || j != poly2.end()){ 
		if(i != poly1.end()){
			exp=i->first;
			coef=i->second;
			i++;
		}
		if(j != poly2.end()){
			exp=j->first;
			coef=-(j->second);
			j++;
		}
		result[exp]=coef;
	}
}


template<class T>
void load(stringstream& ss, map<T, T, greater<int> >& polyCon, ofstream& out){
	
	bool firstTerm=true;
	T c, e;

	while(ss>>c>>e){
		polyCon[e] += c;

//printing original form while loading to the map since map
//keys are unique;
                if(c == 0){ continue;}//if coef is 0, skip;
                else if(c == 1 && firstTerm){out<<""; firstTerm=false;} //print no coef;
                else if(c == 1 && !firstTerm && e == 0){out<<"+1";}
                else if(c == 1 && !firstTerm){out<<"+";}
                else if(c>0 && !firstTerm) {out<<"+"<<c;}
                else if(c == -1 && e != 0) {out<<"-";}
                else {out<<c; firstTerm=false;}   //all other conditions simply print c;

                //exponent part;

                if(e == 0){continue;}    //skip since it's 1;
                else if(e == 1){out<<"x";} //print only x;
                else {out<<"x^"<<e;}
	}
	out<<endl<<endl;
}



template<class T>
void print(map<T, T, greater<int> > poly, ostream& out){
	bool firstTerm=true;
	auto i=poly.begin();

	while(i != poly.end()){
                //coefficient part:
                T c = i->second;
                T e = i->first;

                if(c == 0){ i++; continue;}//if coef is 0, skip;
                else if(c == 1 && firstTerm){out<<""; firstTerm=false;} //print no coef;
                else if(c == 1 && !firstTerm && e == 0){out<<"+1";}
                else if(c == 1 && !firstTerm){out<<"+";}
                else if(c>0 && !firstTerm) {out<<"+"<<c;}
                else if(c == -1 && e != 0) {out<<"-";}
                else {out<<c; firstTerm=false;}   //all other conditions simply print c;

                //exponent part;
                if(e == 0){ i++; continue;}    //skip since it's 1;
                else if(e == 1){out<<"x";} //print only x;
                else {out<<"x^"<<e;}

                i++;
		firstTerm=false;		
	}
	out<<endl<<endl;
}



int main(int argc, char *argv[]){
	map<int, int, greater<int> >poly1, poly2, result;

	// in this program, if the input file is
	// missing a integer, then it is assume to be 1;	
	const char* filename="input.txt";
	const char* outfile="output.txt";

	ifstream infile(filename);
	ofstream out(outfile);

	while(infile){

		int i;
		string s;
		getline(infile, s);  //get first line to s;
		stringstream ss1(s);
		if(ss1.str()=="") break;  //if no more lines, get out;
		out<<"Polynomial1 regular form:       ";
		load<int>(ss1, poly1, out);
		out<<"Polynomial1 canonical form:     ";
		print(poly1, out);

		getline(infile, s);
		stringstream ss2(s);
		out<<"Polynomial2 regular form:       ";
		load<int>(ss2, poly2, out);
		out<<"Polynomial2 canonical form:     ";	
		print(poly2, out);

		sum(poly1, poly2, result);
		out<<"polynomial1 + polynomial2 =     ";
		print(result, out);
		result.clear();		

		difference(poly1, poly2, result);	
		out<<"polynomial1 - polynomial2 =     ";	
		print(result, out);
		result.clear();

		product(poly1, poly2, result);
		out<<"polynomial1 * polynomial2 =     ";
		print(result, out);
		poly1.clear();
		poly2.clear();
		result.clear();

		out<<"+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"<<endl;
		out<<endl;
	}

	infile.close();
	out.close();

	return 0;
}
