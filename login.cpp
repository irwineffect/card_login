#include "student_db.hpp"
#include "card_reader.hpp"
#include <stdlib.h>
#include <signal.h>


//forward declarations
void sig_handler(int signo);

//global variables
Student_db db("student_records.dat", "password");
Card_reader reader(0);

int main (void)
{

	int id;
	string name;

	//setup signal handler
	signal(SIGINT, sig_handler);

	db.Load_records();
	while(1)
	{
		cout << "please swipe card" << endl;
		//get id number from the card reader
		id = atoi(reader.Read().c_str());

		//check if id already is known
		if(!db.Lookup_name(id, name)) //if not, prompt for a name and add to database
		{
			cout << "card not recognized, you must be new!" << endl;
			string tmp;
			while(1)
			{
				cout << "Please enter your name (first last) > ";
				getline(cin, name);
				cout << "you entered \"" << name << "\", is this correct (y/n)? > ";
				getline(cin, tmp);
				if(tmp[0] == 'y')
					break;
			}
			db.Add(id, name);
		}
		cout << "welcome " << name << '!' << endl;;
		db.Login(id); //login the user
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

