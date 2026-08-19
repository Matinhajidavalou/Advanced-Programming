#include <iostream>
#include <string>
using namespace std;

bool recursive_check(const string , int , int);
void process_line(string );
void print_answer(int);
bool is_english(char);
char to_lower(char);

int main(){
	int line_count;
	cin >> line_count;
	cin.ignore();
	for (int i = 0; i < line_count; ++i)
	{
		string line;
		getline(cin , line);
		process_line(line);
	}
}
bool recursive_check(const string input , int start , int end ){
	if(start >= end){return true;}
	if(!is_english(input[start])){
		return recursive_check(input , start+1 , end);
	}
	else if(!is_english(input[end])){
		return recursive_check(input, start ,end -1);
	}
	if(to_lower(input[start]) != to_lower(input [end])){
		return false;}
		else{return recursive_check(input , start + 1 , end -1);
	}
}
void process_line(string input){
	int start , end , input_length;
	start = 0; 
	end = input.length() - 1;
	input_length = input.length();
	if(recursive_check(input , start , end)){print_answer(1);}
	else {print_answer(0);}
}
void print_answer(int answer){
	if (answer == 0){
		cout << "no" << endl;
	}
	else if (answer == 1){
		cout << "yes" << endl;
	}
}
bool is_english(char character){
	if((character >= 'a' && character <= 'z' )||( character >= 'A' && character <= 'Z')){
		return true;
	}
	else {return false;}
	
}
char to_lower(char character){
	if (character >= 'A' && character <= 'Z'){
		character += 32;	} 
		return character;
	}
