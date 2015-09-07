#include <iostream>
#include <termios.h>

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

	private:
		//internal functions
		bool verify_line(string line);
		string extract_id(string line);

		//private members
		termios m_old_term_settings;
		int m_stdin_fileno;
	
};
