#include "student_db.hpp"
#include "card_reader.hpp"



int main (void)
{

	Student_db db("student_records.dat", "password");


	cout <<	"records1" << endl;
	db.Display_records();

	db.Add(42, "John Doe");
	cout <<	"records2" << endl;
	db.Display_records();

	db.Login(32);
	cout <<	"records3" << endl;
	db.Display_records();


	return 0;
}
