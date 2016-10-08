#include "student_db.hpp"
#include "card_reader.hpp"
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>


//forward declarations
void sig_handler(int signo);
void timer_handler(int signo);
long parse_id(string raw_read);

//global variables
Card_reader reader(0);
Student_db db;

int main (int argc, char* argv[])
{

	uint64_t id;
	string name;
	string id_preparse;
	int fd;

	//Timer variables
	timer_t tid;
	sigevent sevp;
	itimerspec its;

	//Setup the SigEvent to trigger SIGUSR1 signal
	sevp.sigev_notify = SIGEV_SIGNAL;
	sevp.sigev_signo = SIGUSR1;

	//Set the Timer to trigger ~every minute
	its.it_interval.tv_sec = 60;
	its.it_interval.tv_nsec = 0;
	its.it_value.tv_sec = 60;
	its.it_value.tv_nsec = 0;

	//setup signal handler
	signal(SIGINT, sig_handler);
	signal(SIGHUP, sig_handler);
	signal(SIGUSR1, timer_handler);

	//Create and set the save timer
	timer_create(CLOCK_REALTIME, &sevp, &tid);
	timer_settime(tid, 0, &its, NULL);

	if(argc < 2)
	{
		cout << "must input filename of database!" << endl;
		exit(1);
	}

	fd = open(argv[1], O_RDONLY);
	if(fd < 0)
	{
		string tmp;
		cout << "file \'" << argv[1] << "\' does not exist, do you want to create it? > ";
		getline(cin, tmp);
		if(tmp[0] != 'y')
			exit(1);
	}
	else
	{
		close(fd);
	}

	db = Student_db(argv[1], "password");

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

					//Loop over the name and send specific characters to upper
					for (auto i = name.begin(); i < name.end(); ++i)
					{
					    if (i == name.begin()) *i = toupper(*i);
					    if (*(i-1) == ' ') *i = toupper(*i);
					    if (*(i-1) == '-') *i = toupper(*i);
					}

					if (cin.eof()) //Someone typed CTRL-D !
					{
						cin.clear(); //Clear the Error bits
						cin.ignore(); //Clear out the input buffer
						continue; //Restart the loop
					}

					cout << "You entered \"" << name << "\", is this correct (y/n)? > ";
					getline(cin, tmp);

					if (cin.eof()) //Someone typed CTRL-D !
					{
						cin.clear(); //Clear the Error bits
						cin.ignore(); //Clear out the input buffer
						continue; //Restart the loop
					}
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
	else if (signo == SIGHUP)
		cout << "catching SIGHUP" << endl;

	reader.Restore_term();
	db.Write_records();

	cout << "exiting!" << endl;
	exit(0);
}

//Wrapper for saving the database when the timer fires
void timer_handler(int signo)
{
	cout << "Caught SIGUSR1" << endl;
	db.Write_records();
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
