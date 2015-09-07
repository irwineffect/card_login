#include <iostream>
#include <termios.h>
#include <time.h>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::getline;
using std::vector;
using std::cerr;
using std::flush;

class Student_db
{
	public:
		Student_db(string db_filename, string password);
		~Student_db(void);

		bool Login(int id);
		bool Lookup_name(int id, string &name);
		bool Add(int id, string name);
		void Display_records(void);

	private:
		typedef struct
		{
			char name[32];
			int id;
			int num_times;
			//time_t times[]; these follow after header
		} Student_file_record_header;

		typedef struct
		{
			string name;
			int id;
			vector<time_t> times;
		} Student_memory_record;

		//internal functions
		string hash_id(string id);
		bool load_records();
		bool write_records();

		//private members
		string m_db_fname;
		string m_salt;
		vector<Student_memory_record> m_students;
	
};
