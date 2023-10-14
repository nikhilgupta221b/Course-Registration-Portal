#ifndef COURSE_STRUCT
#define COURSE_STRUCT

struct course
{
    int courseID;
    char name[25];
    int facultyID;
    int seats;
    int available_seats;
};

#endif