#include "student_db.hpp"
#include <stdlib.h>
#include <signal.h>


//forward declarations
void sig_handler(int signo);

//global variables
Student_db db;

int main (int argc, char *argv[])
{
	//setup signal handler
	signal(SIGINT, sig_handler);

	if(argc < 2)
	{
		cout << "must input filename of database!" << endl;
		exit(1);
	}

	db = Student_db(argv[1], "password");

	db.Load_records();
	db.Display_records();


	return 0;
}


void sig_handler(int signo)
{
	if(signo == SIGINT)
		cout << "catching SIGINT" << endl;

	cout << "exiting!" << endl;
	exit(0);
}
