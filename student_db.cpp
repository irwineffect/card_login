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

bool Student_db::Login(int id)
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

bool Student_db::Lookup_name(int id, string &name)
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

bool Student_db::Add(int id, string name)
{
	string tmp;
	if(Lookup_name(id, tmp))
	{
		cerr << "student id already exists in the database" << endl;
		return false;
	}//else

	Student_memory_record new_student;

	//should check that name is less than max_name size
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

