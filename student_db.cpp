#include "student_db.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Student_db::Student_db(string db_filename, string password)
{
	m_salt = password;
	m_db_fname = db_filename;
}

Student_db::~Student_db(void)
{
}

bool Student_db::Login(long id)
{
	vector<Student_memory_record>::iterator i;

	for(i = m_students.begin(); i != m_students.end(); ++i)
	{
		if(i->id == id)
		{
			time_t current_time = time(0);
			i->times.push_back(current_time);
			return true;
		}
	}

	return false;
}

bool Student_db::Lookup_name(long id, string &name)
{
	vector<Student_memory_record>::iterator i;

	for(i = m_students.begin(); i != m_students.end(); ++i)
	{
		if(i->id == id)
		{
			name = i->name;
			return true;
		}
	}

	return false;
}

bool Student_db::Add(long id, string name)
{
	string tmp;
	if(Lookup_name(id, tmp))
	{
		cerr << "student id already exists in the database" << endl;
		return false;
	}//else

	if(name.length() > 31) //check for name length
	{
		cerr << "name length is too long, truncating!" << endl;
		name = name.substr(0,31);
	}

	Student_memory_record new_student;

	new_student.name = name;
	new_student.id = id;
	new_student.times.clear();

	m_students.push_back(new_student);

	return true;
}

void Student_db::Display_records(void)
{
	vector<Student_memory_record>::iterator i;
	vector<time_t>::iterator j;

	for(i = m_students.begin(); i != m_students.end(); ++i)
	{
		cout << i->id << " " << i->name << endl;
		for(j = i->times.begin(); j != i->times.end(); ++j)
		{
			cout << "\t" << ctime(&(*j)) << endl;
		}
		cout << endl;
	}
}

bool Student_db::Load_records(void)
{
	FILE* db_fh;
	db_fh = fopen(m_db_fname.c_str(), "rb");

	if(db_fh == NULL)
	{
		cerr << "file \"" << m_db_fname << "\" does not exist, it will be created when database is written out" << endl;
		return false;
	}

	
	//load student records into memory
	cout << "loading student records..." << flush;
	m_students.clear();
	int num_records=0;;

	Student_file_record_header header;

	while(fread(&header, sizeof(header), 1, db_fh)) //read header of student record
	{
		//allocate memory for time records
		time_t *times = new time_t [header.num_times];

		//read times into memory
		fread(times, sizeof(time_t), header.num_times, db_fh);

		//load student into db_structure
		Student_memory_record student;
		student.name = header.name;
		student.id = header.id;
		for(int i=0; i < header.num_times; ++i)
		{
			time_t tmp = times[i];
			student.times.push_back(tmp);
		}
		m_students.push_back(student);
		delete [] times;
		++num_records;
	}

	cout << num_records << " student records loaded" << endl;

	fclose(db_fh);

	return true;
}


bool Student_db::Write_records(void)
{
	FILE* db_fh = fopen(m_db_fname.c_str(), "wb");

	if(db_fh == NULL)
	{
		cerr << "could not open db file for write, aborting!" << endl;
		exit(1);
	}

	//write student records to file 
	cout << "writing student records..." << flush;
	int num_records=0;
	
	vector<Student_memory_record>::iterator i;
	vector<time_t>::iterator j;

	Student_file_record_header header;

	for(i = m_students.begin(); i != m_students.end(); ++i)
	{
		//fill out the header
		strcpy(header.name, i->name.c_str());
		header.id = i->id;
		header.num_times = i->times.size();
		//write the header to the file
		fwrite(&header, sizeof(header), 1, db_fh);

		//write times to the file
		for(j = i->times.begin(); j != i->times.end(); ++j)
		{
			time_t time = *j;
			fwrite(&time, sizeof(time), 1, db_fh); 
		}
		++num_records;
	}

	cout << num_records << " student records written" << endl;
	fclose(db_fh);
	return true;
}

double Student_db::Get_student_frequency(long id)
{
	string student_name;
	int totalDays = 0, flag = 0;
	int personIndex = -1, index = 0;
	vector<int> attendance;
	struct tm *time_struct;
	//ensure the student exists
	if (this->Lookup_name(id, student_name) == 0) 
	{
		cout << "Student not found." << endl;
		return 0;
	}
	//if they exist, determine total days

	totalDays = this->Get_total_days();
	
	//find their person index
	while (personIndex = -1 && index < m_students.size())
	{
		if (m_students.at(index).name == student_name) personIndex = index;
		index++;
	}

	//determine number of days that they were present
	for (int i = 0; i < m_students.at(personIndex).times.size(); i++) 
	{
		time_struct = localtime(&m_students.at(personIndex).times.at(i));	
		int tmp = time_struct->tm_year*1000 + time_struct->tm_yday;
		flag = 0;
		for (int j = 0; j < attendance.size(); j++)
		{
			if (attendance.at(j) == tmp) flag = 1;
		}
		if (!flag) attendance.push_back(tmp);
	}
	return ((double)attendance.size())/totalDays;
}

int Student_db::Get_total_days(void)
{
	int tempDate;
	int flag = 0;
	struct tm *time_struct;
	vector<int> days;
   days.clear();	
	//This function will determine the total number of days of unique days within the database
	for (int student = 0; student < m_students.size(); student++)
	{
		for (int time = 0; time < m_students.at(student).times.size(); time++)
		{
			//check if the current time exists in the days vector
			time_struct = localtime(&m_students.at(student).times.at(time));
			
			tempDate = time_struct->tm_yday + time_struct->tm_year*1000;
			flag = 0;

			for (int k = 0; k < days.size(); k++)
			{
				if (days.at(k) == tempDate) flag = 1;
			}

			if (!flag) 
			{
				days.push_back(tempDate);
			}
		}
	}	
	return days.size();
}
