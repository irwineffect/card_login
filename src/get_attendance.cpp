#include "student_db.hpp"

int main()
{
	Student_db db("student_records.dat", "password");
	db.Load_records();

	double attendance = 0;
	attendance = db.Get_student_frequency(6008770113799401);
	cout << endl << attendance << endl;
}
