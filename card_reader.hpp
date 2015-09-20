#include <iostream>
#include <termios.h>
#include <stdio.h>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::getline;

class Card_reader
{
	public:
		Card_reader(int stdin_fileno);
		~Card_reader(void);
		string Read(void);
		void Restore_term(void);

	private:
		//internal functions
		bool verify_line(string line);
		string extract_id(string line);
		void setup_term(void);

		//private members
		termios m_old_term_settings;
		termios m_new_term_settings;
		int m_stdin_fileno;
		FILE* log_fh;
	
};
