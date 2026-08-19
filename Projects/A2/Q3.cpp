#include <iostream>
#include <string>
#include <vector>

using namespace std;

string location(int , int);
bool moves(vector<vector<char>>&, vector<string>&);
bool game_check(vector<vector<char>>&, vector<string>&);

int main(){
	vector<vector<char>> map (7 , vector<char>(7));
	vector <string> answers;

	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			cin >> map[i][j];
		}
	}


	if(game_check(map , answers)){
		for (int i = 0; i < answers.size(); ++i)
		{
			cout<< answers[i] << endl;
		}
	}
	else{
		cout<< "Loser" << endl;
	}
	return 0;
}
string location(int i , int j){
	string s = "";
	s+= (char)('A' + j);
	s+= (char)('1' + i);
	return s;
}

bool moves(vector<vector<char>>& map, vector <string> &answers){
const int column_dir [] = {-1 , 1 , 0 , 0};
	const int row_dir [] = {0 , 0 , -1 , 1};
	const string direction[] = {"UP" , "DOWN" , "LEFT" , "RIGHT"};
	for(int i=0 ; i < 7 ; i++){
		for (int j = 0 ; j < 7 ; j++){
			if(map[i][j] == 'N'){
				for(int k = 0 ; k < 4 ; k++ ){
					int first_jumpi= i + column_dir[k];
					int first_jumpj= j + row_dir[k];
					int second_jumpi = i + 2*column_dir[k];
					int second_jumpj = j + 2*row_dir[k];

					if(second_jumpi>= 0 && second_jumpi<7 && second_jumpj >= 0 && second_jumpj<7){
						 if(first_jumpi >= 0 && first_jumpi < 7 && first_jumpj >= 0 && first_jumpj < 7) {
						if (map[first_jumpi][first_jumpj ] == 'N' && map[second_jumpi][second_jumpj] == 'O'){
							map[i][j] = 'O'; map[first_jumpi][first_jumpj] = 'O';
							map[second_jumpi][second_jumpj] ='N';
							answers.push_back (location(i , j) + " " + direction[k]);
							if(game_check(map , answers)){return true;}

							map[i][j] = 'N'; map[first_jumpi][first_jumpj] = 'N' ; map[second_jumpi][second_jumpj]= 'O';
							answers.pop_back();
						} 
					}}
				}
			}
			
		}
	}
	return false;


}
bool game_check(vector<vector<char>>& map, vector <string>& answers){
	
	int count_N = 0;
	
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			if(map[i][j] == 'N'){
				count_N ++;
			}
		}
	}


if (count_N == 1 && map[3][3] == 'N'){return true;}
else if (count_N == 1 && map[3][3] != 'N') {return false;}

return moves(map , answers);

}

