#include "student_db.hpp"
#include <stdlib.h>
#include <signal.h>


//forward declarations
void sig_handler(int signo);

//global variables
Student_db db("student_records.dat", "password");

int main (void)
{
	//setup signal handler
	signal(SIGINT, sig_handler);

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

