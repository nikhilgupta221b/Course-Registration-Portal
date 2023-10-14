/*
============================================================================
Name : server.c
Author : Nikhil Gupta 
Description : This file consists of code which listens for client connections
              and connect to it through socket. Used forking for handling multiple clients  
============================================================================
*/


#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h> 
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <errno.h>

#include"./Structures/student.h"
#include"./Structures/faculty.h"
#include"./Structures/admin.h"
#include"./Structures/course.h"

#include"./Functions/student_func.h"
#include"./Functions/faculty_func.h"


// check admin
bool checkAdmin(struct admin currUser)
{
    int i = currUser.userID;
    int fd = open("/home/nikhil/Academia/Database/Admin.data", O_RDONLY, 0744);
    bool result;
    struct admin temp;

    int fl1;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i) * sizeof(struct admin);
    lock.l_len = sizeof(struct admin);
    lock.l_pid = getpid();

    fl1 = fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i) * sizeof(struct admin), SEEK_SET);
    read(fd, &temp, sizeof(struct admin));
    if (!strcmp(temp.password, currUser.password))
        result = true;
    else
        result = false;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

void serverTask(int nsd)
{
    int msgLength, select, type, option, userType, currUserID;
    bool result;
    // login verification
    while (1)
    {
        // read what client has chosen from initial menu
        read(nsd, &option, sizeof(option));
        printf("Option : %d\n", option);
        // if student
        if (option == 1)
        {
            struct student currUser1;
            userType = 1;
            msgLength = read(nsd, &currUser1, sizeof(struct student));
            printf("Username : %d\n", currUser1.userID);
            printf("Password : %s\n", currUser1.password);
            currUserID = currUser1.userID;
            result = checkStudent(currUser1);
            write(nsd, &result, sizeof(result));
        }
        else if (option == 2)
        {
            struct faculty currUser2;
            userType = 2;
            msgLength = read(nsd, &currUser2, sizeof(struct faculty));
            currUserID = currUser2.userID;
            printf("Username : %d\n", currUser2.userID);
            printf("Password : %s\n", currUser2.password);
            result = checkFaculty(currUser2);
            write(nsd, &result, sizeof(result));
        }
        else if (option == 3)
        {
            struct admin currUser3;
            userType = 3;
            msgLength = read(nsd, &currUser3, sizeof(struct admin));
            currUserID = currUser3.userID;
            printf("Username : %d\n", currUser3.userID);
            printf("Password : %s\n", currUser3.password);
            result = checkAdmin(currUser3);
            write(nsd, &result, sizeof(result));
        }
        else {
            result = false;
            write(nsd,&result,sizeof(result));
        }
        if (result) break;
    }
    // internal menus
    while(1){
        read(nsd,&select,sizeof(int));
        // student
        if (option == 1){
            if (select == 1){
                
            } 
            else if (select == 2){
                
            } 
            else if (select == 3){
                
            }
            // 
            else if (select == 4){
                
            }
            // change student password
            else if (select == 5){
                struct student modifyStudent;
                read(nsd,&modifyStudent,sizeof(struct student));
                result=changeStudentPass(modifyStudent);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 6) break;
        }
        // faculty
        else if (option == 2){
            //view offered courses
            if (select == 1){
                int facultyID;
                read(nsd,&facultyID,sizeof(int));
                viewOfferedCourses(facultyID, nsd);
            }
            // faculty add new course
            else if (select == 2){
                struct course addCourse;
                read(nsd,&addCourse,sizeof(struct course));
                result=addNewCourse(addCourse, nsd);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 3){
                int courseID;
                read(nsd,&courseID,sizeof(int));
                deleteCourse(courseID, nsd);
            }
            // update course details
            else if (select == 4){
                struct course modifyCourse;
                read(nsd,&modifyCourse,sizeof(struct course));
                result=updateCourseDetails(modifyCourse);
                write(nsd,&result,sizeof(result));
            } 
            // change faculty password
            else if (select == 5){
                struct faculty modifyFaculty;
                read(nsd,&modifyFaculty,sizeof(struct faculty));
                result=changeFacultyPass(modifyFaculty);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 6) break;
        }
        // admin
        else if (option == 3){
            if (select == 1){
                struct student newStudent;
                read(nsd,&newStudent,sizeof(struct student));
                result = addStudent(newStudent, nsd);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 2){
                struct student searchStudent;
                int userID;
                read(nsd,&userID,sizeof(int));
                searchStudent = searchStudentRecord(userID);
                write(nsd,&searchStudent,sizeof(struct student));
            } 
            else if (select == 3){
                struct faculty newFaculty;
                read(nsd,&newFaculty,sizeof(struct faculty));
                result = addFaculty(newFaculty, nsd);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 4){
                struct faculty searchFaculty;
                int userID;
                read(nsd,&userID,sizeof(int));
                searchFaculty = searchFacultyRecord(userID);
                write(nsd,&searchFaculty,sizeof(struct faculty));
            } 
            else if (select == 5){
                struct student activateStudent;
                read(nsd,&activateStudent,sizeof(struct student));
                result=activateStudentStatus(activateStudent);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 6){
                struct student blockStudent;
                read(nsd,&blockStudent,sizeof(struct student));
                result=blockStudentStatus(blockStudent);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 7){
                struct student modifyStudent;
                read(nsd,&modifyStudent,sizeof(struct student));
                result=updateStudentDetails(modifyStudent);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 8){
                struct faculty modifyFaculty;
                read(nsd,&modifyFaculty,sizeof(struct faculty));
                result=updateFacultyDetails(modifyFaculty);
                write(nsd,&result,sizeof(result));
            } 
            else if (select == 9) break;
        }
    }
    close(nsd);
    write(1, "Client session Ended\n", sizeof("Client session Ended\n"));
    return;
}
int main()
{
    struct sockaddr_in server, client;
    int sd, nsd, clientLen;
    bool result;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5555);
    bind(sd, (struct sockaddr *)&server, sizeof(server));
    listen(sd, 5);
    printf("Welcome to Academia\n");
    printf("Waiting for Client to connect\n");
    while (1)
    {
        clientLen = sizeof(client);
        nsd = accept(sd, (struct sockaddr *)&client, &clientLen);

        write(1, "Connected to the client\n", sizeof("Connected to the client\n"));
        if (!fork())
        {
            close(sd);
            serverTask(nsd);
            exit(0);
        }
        else
        {
            close(nsd);
        }
    }
    return 0;
}
