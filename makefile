OPTIONS=-Wall -g
CC=g++

all: login.exe 

reader.exe: readcard.cpp 
	$(CC) $(OPTIONS) readcard.cpp -o reader.exe

card_reader.o: card_reader.hpp card_reader.cpp
	$(CC) $(OPTIONS) -c card_reader.cpp	

student_db.o: student_db.hpp student_db.cpp
	$(CC) $(OPTIONS) -c student_db.cpp	

login.exe: student_db.o card_reader.o login.cpp
	$(CC) $(OPTIONS) student_db.o card_reader.o login.cpp -o login.exe

read_db.exe: student_db.o card_reader.o read_db.cpp
	$(CC) $(OPTIONS) student_db.o card_reader.o read_db.cpp -o read_db.exe

clean:
	rm -f *.o *.exe
