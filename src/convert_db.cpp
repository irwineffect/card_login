// C/C++ File
// AUTHOR:   Brandon Kallaher
// FILE:     convert_db.cpp
// CREATED:  2016-10-07 19:37:05
// MODIFIED: 2016-10-07 20:29:19
// DESC:     TODO (some explanation)

#include <errno.h>
#include "student_db.hpp"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <chrono>

typedef struct
{
    char name[32];
    long id;
    int num_times;
} Student_file_record_header_old;

int main (int argc, char* argv[])
{
    FILE *old_db_fh, *new_db_fh;
    uint64_t id;
    string name;
    Student_db db;
    int fd;

    if (argc < 2)
    {
        cout << "must input filename of database!" << endl;
        exit(1);
    }

    if ((fd = open(argv[1], O_RDONLY) < 0))
    {
        errno = ENOENT;
        perror("[conver_db (23)]");
    }
    else close(fd);
    name = argv[1];
    db = Student_db((name+".new").c_str(), "password");
    
    old_db_fh = fopen(argv[1], "r");

    if (old_db_fh == NULL)
    {
        cerr << "File \"" << argv[1] << "\" does not exist!" << endl;
        exit(1);
    }

    cout << "Loading and updating records..." << flush;
    db.m_students.clear();
    int num_records = 0;

    Student_file_record_header_old header_old;

    while (fread(&header_old, sizeof(header_old), 1, old_db_fh))
    {
        time_t *times_old = new time_t [header_old.num_times];

        fread(times_old, sizeof(time_t), header_old.num_times, old_db_fh);

        Student_db::Student_memory_record student;
        student.name = header_old.name;
        student.id = header_old.id;
        for (int i=0; i < header_old.num_times; ++i)
        {
            time_t tmp = times_old[i];
            student.times.push_back(system_clock::from_time_t(tmp));
        }
        db.m_students.push_back(student);
        delete [] times_old;
        ++num_records;
    }

    db.Write_records();

    return 0;
}
