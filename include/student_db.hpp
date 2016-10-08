#include <iostream>
#include <termios.h>
#include <time.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctype.h>

using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::getline;
using std::vector;
using std::cerr;
using std::flush;
using std::chrono::system_clock;
using std::uint64_t;

class Student_db
{
    public:
		typedef struct
		{
			char name[32];
            uint64_t id;
			int num_times;
			//time_t times[]; these follow after header
		} Student_file_record_header;

		typedef struct
		{
			string name;
			uint64_t id;
            vector<system_clock::time_point> times;
		} Student_memory_record;

		//internal functions
		string hash_id(string id);
		//private members
		string m_db_fname;
		string m_salt;
		
        Student_db(void){}
		Student_db(string db_filename, string password);
		~Student_db(void);

		bool Login(uint64_t id);
        bool Lookup_name(uint64_t id, string &name);
        bool Add(uint64_t id, string name);
		void Display_records(void);
		bool Load_records();
		bool Write_records();

		//Frequency functions
        double Get_student_frequency(uint64_t id);
		int Get_total_days(void);
		vector<int> Get_all_days();
		
        //Student Records
        vector<Student_memory_record> m_students;

		//Functions created for exporting database to CSV
		int Get_record_count();
        uint64_t Get_ID(int index);
        bool Check_attendance(uint64_t id, int year_yday);

};
