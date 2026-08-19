#include <iostream>
#include <string>
using namespace std;

bool is_english(char);
bool is_digit(char);
string repeat(const string&, int);
string process(const string& , int&);

int main (){
	string input;
	while (true){
	getline(cin , input);
	int pos = 0;
	cout << process(input , pos) << endl;}
	return 0;
}

bool is_english(char character){
	if((character >= 'a' && character <= 'z' )||( character >= 'A' && character <= 'Z')){
		return true;
	}
	else {return false;}
	
}


bool is_digit(char character){
	if(character >= '0' && character <= '9'){
		return true;
	}
	else {return false;}
}




string repeat(const string& input , int k){
	if(k==0) {return "";}
	if(k==1) {return input;}
	if(k>1) {return input + repeat(input , k -1);}
	return "";
}

string  process(const string& input , int& pos){
	string result;
	
	while(pos < input.length() && input[pos] != ']'){
		if (is_english(input[pos])){
			result += input[pos];
			pos ++; 
		}
		
		else if (is_digit(input[pos])){
			int k = 0;
			while(input[pos] != '['){
				k= k*10 + (input[pos] - '0');
				pos++;
			}


			if (input[pos] == '[')
			{
				pos ++;
			}

			string temp = process(input , pos );
			result += repeat(temp , k);
		}
	}	
	if (pos< input.length() && input[pos] == ']'){pos ++;}

	return result;
}