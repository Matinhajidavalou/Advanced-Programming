#include <iostream>
#include <string>
#include <map>

using namespace std;

const map <int , int > hallCapaticies = {
	{1 , 10}, {2, 20} , {3 , 30}
};

struct Show {
	string movieName;
	int time;
	map<int , string> reservedSeat;
};

map <int , map<int , Show >> shows;

void addShow(string , int , int);
void reserveSeat(int , int , int , string);
void listMovies(int);
void reportSeats(int , int );

int main() {

	string command;
	while (cin >> command)
		if (command == "add_show"){
			string movieName;
			int hallNumber , time;
			cin >> movieName >> time >>hallNumber;
			addShow(movieName , time , hallNumber);

		}
		else if(command == "reserve")
		{
			int hallNumber , time , seatNumber;
			string name;
			cin >> hallNumber >> time >> seatNumber >> name;
			reserveSeat(hallNumber , time , seatNumber , name);
		}

		else if (command == "list_movies")
		{
			int hallNumber;
			cin >> hallNumber;
			listMovies(hallNumber);
		}

		else if (command == "report_seats")
		{
			int hallNumber , time;
			cin >> hallNumber >> time;
			reportSeats(hallNumber , time);
		}

		return 0;
	}

	void addShow(string movieName , int time , int hallNumber){
		if (shows[hallNumber].count(time) > 0){
			cout << "A movie is already scheduled in this hall at this time\n";
		}
		else{

			Show newShow;
			newShow.movieName = movieName;
			newShow.time = time;
			shows[hallNumber][time] = newShow;
			cout << "OK\n" ;
		}}
		void reserveSeat(int  hallNumber , int time , int seatNumber , string name){

			if (shows[hallNumber].count(time) == 0)
			{
				cout << "No show is scheduled in this hall at the specified time\n";
				return;
			}
			Show& currentShow = shows[hallNumber][time];
			if (shows[hallNumber].count(time) > 0 && currentShow.reservedSeat.count(seatNumber) > 0)
			{
				cout<< "This seat is already reserved\n"	;
			}
			else{
				currentShow.reservedSeat[seatNumber]= name; 
				cout << "OK\n" ;
			}
		}
		void listMovies(int hallNumber){
			int showsCount = shows.count(hallNumber);
			if(showsCount== 0 ){
				cout << "No movie found\n";
				return;
			}
			else{
				for (auto const& [time , Show] : shows[hallNumber])
				{

					int hallCapaticie = hallCapaticies.at(hallNumber);
					int reservedSeats = Show.reservedSeat.size();
					int availableSeats = hallCapaticie - reservedSeats;
					cout << Show.movieName <<" at "<< time << ":00: "<<availableSeats<<" seats available\n";
				}
			}

		}
		void reportSeats(int hallNumber , int time){

			if(shows[hallNumber].count(time) == 0 || shows[hallNumber][time].reservedSeat.empty()){
				cout << "All seats are available\n";
			}
			else {
				for (auto const& [seatNumber , name]: shows[hallNumber][time].reservedSeat  )
				{
					cout << "Seat " << seatNumber << " is reserved by " << name << endl;
				}
			}
		}