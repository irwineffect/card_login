#include "student_db.hpp"

int main()
{
	Student_db db("student_records.dat", "password");
	db.Load_records();

	db.Display_records();

}
