#include "student_db.hpp"
#include "card_reader.hpp"
#include <stdlib.h>
#include <signal.h>


//forward declarations
void sig_handler(int signo);
long parse_id(string raw_read);

//global variables
Student_db db("student_records.dat", "password");
Card_reader reader(0);

int main (void)
{

	long id;
	string name;
	string id_preparse;
	//setup signal handler
	signal(SIGINT, sig_handler);

	db.Load_records();
	while(1)
	{
		cout << "Please swipe card" << endl;
		//get id number from the card reader
		id = parse_id(reader.Read_raw().c_str());
		if (id != 0) 
		{
			//check if id already is known
			if(!db.Lookup_name(id, name)) //if not, prompt for a name and add to database
			{
				cout << "Card not recognized, you must be new!" << endl;
				string tmp;
				while(1)
				{
					cout << "Please enter your name (first last) > ";
					getline(cin, name);
					cout << "You entered \"" << name << "\", is this correct (y/n)? > ";
					getline(cin, tmp);
					if(tmp[0] == 'y')
						break;
				}
				db.Add(id, name);
			}
			cout << "Welcome " << name << "! You have been logged in." << endl << endl;
			db.Login(id); //login the user
		} 
		else
		{
			cout << "Error. Please try again." << endl;
		}
	}

	return 0;
}


void sig_handler(int signo)
{
	if(signo == SIGINT)
		cout << "catching SIGINT" << endl;

	db.Write_records();
	reader.Restore_term();

	cout << "exiting!" << endl;
	exit(0);
}

long parse_id(string raw_read)
{
	int index_semicolon = -1, index_equals = -1;
	string temp;

	//This will first search the string for ';'
	index_semicolon = raw_read.find_first_of(';', 0);
	index_equals = raw_read.find_first_of('=', index_semicolon);

	if ((index_semicolon == -1) || (index_equals == -1) ) 
	{
		return 0;
	}
	temp = raw_read.substr(index_semicolon+1, index_equals - index_semicolon - 1);

	return strtol(temp.c_str(), NULL, 10);
}

