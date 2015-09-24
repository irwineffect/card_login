#include "card_reader.hpp"
#include <stdlib.h>
#include <signal.h>


//forward declarations
void sig_handler(int signo);

//global variables
Card_reader reader(0);

int main (void)
{
	string line;
	FILE* logf = fopen("cardlogs.log", "a");

	//setup signal handler
	signal(SIGINT, sig_handler);

	reader.Setup_term();	
	while(1)
	{
		cout << "Please swipe card" << endl;
		getline(cin, line);
		line += '\n';
		fwrite(line.c_str(), line.length(), 1, logf);

		cout << "Welcome! You have been logged." << endl << endl;
	}

	return 0;
}


void sig_handler(int signo)
{
	if(signo == SIGINT)
		cout << "catching SIGINT" << endl;

	reader.Restore_term();

	cout << "exiting!" << endl;
	exit(0);
}

