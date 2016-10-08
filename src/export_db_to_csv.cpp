#include "student_db.hpp"
#include <string.h>
#include <stdio.h>

using namespace std;
void convertYday(int yearday, int year, int *month, int *day);

int main(int argc, char *argv[])
{
	string output_filename, database;

	//This function will parse all of the database and export it to a CSV file. The file should be specified in the input arguments
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-file") == 0)
		{
			//Copy the file name into our array
			if (i+1 < argc)
			{
				output_filename = argv[++i];
			}
			else
			{
				cout << "Error. File not specified." << endl;
				return 0;
			}
		} 
		else if (strcmp(argv[i], "-database") == 0)
		{
			if (i+1 < argc)
			{
				database = argv[++i];
			}
			else
			{
				cout << "Error. No database input." << endl;
				return 0;
			}
		}
		else 
		{
			cout << "Database CSV export \nProper Input parameters: \n\t-file [output filename]\n\t-database [database filename]" << endl;
			return 0;
		}
	}

	//We have now properly parsed the program input to get the IO filenames.

	//Open the database and parse it
	Student_db db(database.c_str(), "password");
	if (db.Load_records() == false)
	{
		cout << "Error. Could not load database from specified database file." << endl;
		return 0;
	}

	//Next, attempt to open the output file
	FILE * output_file = fopen(output_filename.c_str(), "w");
	if (output_file == NULL)
	{
		cout << "Error. Could not open the output file." << endl;
		return 0;
	}

	//The respective files have been opened and the database has been opened.
	//Next, begin to parse the database and create a record
	vector<int> days_vector = db.Get_all_days();

	//Next, perform a sort on the vector to order the days in chronological order
	sort(days_vector.begin(), days_vector.end()); //uses std::sort
//	cout << "Vector properly sorted!" << endl;
	int year, month, day, yday;

	//Now, write the header of the file
	fprintf(output_file, "Name, Attendance Frequency");
	for (int i = 0; i < days_vector.size(); i++)
	{
//		cout << days_vector.at(i) << endl;
		//Parse the number in vector into year, day, and month
		year = days_vector.at(i)/1000 + 1900;
		yday = days_vector.at(i)%1000;
		convertYday(yday, year, &month, &day);
		fprintf(output_file, ", %d/%d/%d", month, day, year);
	}
//	cout << "Successful header creation!" << endl;
	uint64_t id;
	string name;
	double frequency;
// 	cout << "Record count: " << db.Get_record_count() << endl;
	//Headers are created. Next, loop through the number of individuals in the database
	for (int i = 0; i < db.Get_record_count(); i++)
	{
		fprintf(output_file, "\n");

		//Write their name and respective frequency
		id = db.Get_ID(i);
		db.Lookup_name(id, name);
		frequency = db.Get_student_frequency(id);
		fprintf(output_file, "%s, %lf", name.c_str(), frequency);
//		cout << "Processing User: " << name << endl;
		//Next, mark them as present
		for (int j = 0; j < days_vector.size(); j++)
		{
			//Loop through each day and check if they were present
			if (db.Check_attendance(id, days_vector.at(j)))
			{
				fprintf(output_file, ", Present");
			}
			else
			{
				fprintf(output_file, ", Absent");
			}
		}
	}

	//close the file as cleanup
	fclose(output_file);
	printf("Successfully wrote attendance records to %s! Exitting.\n", output_filename.c_str());
	return 1;
}

void convertYday(int yearday, int year, int *month, int *day)
{
	*month = 1;
	*day = 0;
	int daysCount = 0;
	int found = 0;
	//April, June, September, November have 30 days
	while (((yearday - daysCount) > 0) && !found)
	{
		switch (*month)
		{
			case 1:
			case 3: 
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
				if (yearday - (daysCount+ 31) < 0)
				{
					//month is found.
					found = 1;
				}
				else
				{
					//continue
					daysCount += 31;
					*month = *month + 1;
				}
				break;
			case 2:
				if (year%4 == 0) //leap year
				{
					if (yearday - (daysCount+ 29) < 0)
					{
						//month is found.
						found = 1;
					}
					else
					{
						//continue
						daysCount += 29;
						*month = *month + 1;
					}
				} else {
					if (yearday - (daysCount+ 28) < 0)
					{
						//month is found.
						found = 1;
					}
					else
					{
						//continue
						daysCount += 28;
						*month = *month + 1;
					}
				}
				break;
			default:
				if (yearday - (daysCount+ 30) < 0)
				{
					//month is found.
					found = 1;
				}
				else
				{
					//continue
					daysCount += 30;
					*month = *month + 1;
				}
				break;
		}	
	}

	*day = yearday - daysCount;
}
