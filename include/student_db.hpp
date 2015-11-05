#include <iostream>
#include <termios.h>
#include <time.h>
#include <vector>
#include <algorithm>

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

		bool Login(long id);
		bool Lookup_name(long id, string &name);
		bool Add(long id, string name);
		void Display_records(void);
		bool Load_records();
		bool Write_records();
	
		double Get_student_frequency(long id);
		int Get_total_days(void);

	private:
		typedef struct
		{
			char name[32];
			long id;
			int num_times;
			//time_t times[]; these follow after header
		} Student_file_record_header;

		typedef struct
		{
			string name;
			long id;
			vector<time_t> times;
		} Student_memory_record;

		//internal functions
		string hash_id(string id);
		//private members
		string m_db_fname;
		string m_salt;
		vector<Student_memory_record> m_students;
	
};
