#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

#define STDIN_FILENO 0

termios oldterm, newterm;

void sig_handler(int signo)
{
	if (signo == SIGINT)
		cout << "catching SIGINT" << endl;

	cout << "restoring terminal settings...";
	tcsetattr(STDIN_FILENO, TCSANOW, &oldterm);
	cout << "done" << endl;

	cout << "exiting!" << endl;

	exit(0);
}

bool verify_line(string line);
string extract_id(string line);
string hash_id(string id);

int main (void)
{
	string line;

	//setup signal handler
	signal(SIGINT, sig_handler);

	//setup the terminal
	tcgetattr(STDIN_FILENO, &oldterm);
	newterm = oldterm;
	newterm.c_lflag &= ~ECHO;
	tcsetattr(STDIN_FILENO, TCSANOW, &newterm);

	while(1)
	{
		getline(cin, line);
		if(!verify_line(line))
		{
			cout << "card not recognized, please swipe again" << endl;
			continue;
		}//else, card recognized
	
		string id = extract_id(line);	
		cout << id << endl;
		cout << "hash: " << hash_id(line) << endl;	
	}

	return 0;
}

bool verify_line(string line)
{
	//confirm line length
	if(line.length() != 95)
		return false;

	//variety of placeholder markers for verification
	//that it is a wsu id card
	if(line[0] != '%')
		return false;
	if(line[11] != '^')
		return false;
	if(line[14] != '?')
		return false;
	if(line[15] != ';')
		return false;
	if(line[32] != '=')
		return false;
	if(line[40] != '=')
		return false;
	if(line[53] != '?')
		return false;
	if(line[54] != '+')
		return false;
	if(line[71] != '=')
		return false;
	if(line[81] != '=')
		return false;

	return true;
}

string extract_id(string line)
{

	return line.substr(23, 8);
}

string hash_id(string id)
{
#define PIPE_READ 0
#define PIPE_WRITE 1

	pid_t pid;
	int inpipe[2];
	int outpipe[2];
	char readbuf[64];

	pipe(inpipe);
	pipe(outpipe);

	pid = fork();

	if (pid < 0) //failed to fork
	{
		cout << "could not fork process to hash ID" << endl;
		exit(1);
	}
	else if (pid) //parent
	{
		int status;
		close(inpipe[PIPE_READ]);
		close(outpipe[PIPE_WRITE]);
		write(inpipe[PIPE_WRITE], id.c_str(), id.length()); //write id to child's stdin
		close(inpipe[PIPE_WRITE]);
		wait(&status); //wait for the child to complete
		read(outpipe[PIPE_READ], readbuf, 64); //read resulting hash

		close(outpipe[PIPE_READ]);
		
		return string(readbuf);
	}
	else //child
	{
		char *args[2];
		args[0] = "/usr/bin/sha256sum";
		args[1] = NULL;
		
		
		close(inpipe[PIPE_WRITE]);
		close(outpipe[PIPE_READ]);
		//connect pipes to stdin and stdout
		close(0);
		dup(inpipe[PIPE_READ]);
		close(1);
		dup(outpipe[PIPE_WRITE]);
		//launch process for child	
		execve("/usr/bin/sha256sum", args, NULL);

		cout << "executing sha256sum failed" << endl;
		exit(1);
	}

}
