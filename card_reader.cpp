#include "card_reader.hpp"

Card_reader::Card_reader(int stdin_fileno)
{
	m_stdin_fileno = stdin_fileno;
	log_fh = fopen("cards.log", "a");

	tcgetattr(stdin_fileno, &m_old_term_settings);
	m_new_term_settings = m_old_term_settings;
	m_new_term_settings.c_lflag &= ~ECHO;
}

Card_reader::~Card_reader(void)
{
	fclose(log_fh);
}

string Card_reader::Read(void)
{
	string line;
	string logline;

	setup_term();
	while(1)
	{
		getline(cin, line);
		logline = line + '\n';
		fwrite(logline.c_str(), logline.length(), 1, log_fh);

		if(!verify_line(line))
		{
			cout << "card not recognized, please swipe again" << endl;
			continue;
		}//else, card recognized
	
		Restore_term();
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

void Card_reader::setup_term(void)
{
	tcsetattr(m_stdin_fileno, TCSANOW, &m_new_term_settings);
}

void Card_reader::Restore_term(void)
{
	tcsetattr(m_stdin_fileno, TCSANOW, &m_old_term_settings);
}
