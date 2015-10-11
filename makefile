OPTIONS=-Wall -g
CC=g++

all: login.exe read_db.exe logger.exe

#
#object files
#
card_reader.o: card_reader.hpp card_reader.cpp
	$(CC) $(OPTIONS) -c card_reader.cpp	 -o build/card_reader.o

student_db.o: student_db.hpp student_db.cpp
	$(CC) $(OPTIONS) -c student_db.cpp	-o build/student_db.o


#
#executables
#
login.exe: student_db.o card_reader.o login.cpp
	$(CC) $(OPTIONS) build/student_db.o build/card_reader.o	login.cpp -o build/login.exe

read_db.exe: student_db.o card_reader.o read_db.cpp
	$(CC) $(OPTIONS) build/student_db.o build/card_reader.o read_db.cpp -o build/read_db.exe

logger.exe: card_reader.o logger.cpp
	$(CC) $(OPTIONS) build/card_reader.o logger.cpp -o build/logger.exe

clean:
	rm -f *.o *.exe
