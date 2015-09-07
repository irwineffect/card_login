#include "card_reader.hpp"

Card_reader::Card_reader(int stdin_fileno)
{
	termios newterm;

	m_stdin_fileno = stdin_fileno;

	tcgetattr(stdin_fileno, &m_old_term_settings);
	newterm = m_old_term_settings;
	newterm.c_lflag &= ~ECHO;
	tcsetattr(stdin_fileno, TCSANOW, &newterm);
}

Card_reader::~Card_reader(void)
{
	tcsetattr(m_stdin_fileno, TCSANOW, &m_old_term_settings);
}

string Card_reader::Read(void)
{
	string line;

	while(1)
	{
		getline(cin, line);

		if(!verify_line(line))
		{
			cout << "card not recognized, please swipe again" << endl;
			continue;
		}//else, card recognized
	
		return extract_id(line);	
	}
}

bool Card_reader::verify_line(string line)
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

string Card_reader::extract_id(string line)
{
	return line.substr(23, 8);
}

