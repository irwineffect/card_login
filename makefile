OPTIONS=-Wall -g
CC=g++

all: reader.exe

reader.exe: readcard.cpp 
	$(CC) $(OPTIONS) readcard.cpp -o reader.exe

mystring.o: mystring.h mystring.cpp
	$(CC) $(OPTIONS) -c mystring.cpp	

clean:
	rm -f *.o *.exe
