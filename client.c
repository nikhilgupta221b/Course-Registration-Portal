/*
============================================================================
Name : client.c
Author : Nikhil Gupta
Description : This file consists of client code which connects to the server
              and give inputs for further processing
============================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>

#include "./Structures/student.h"
#include "./Structures/faculty.h"
#include "./Structures/admin.h"
#include "./Structures/course.h"
#include "./Structures/enrollment.h"

void red()
{
    printf("\033[1;91m");
}
void green()
{
    printf("\033[1;92m");
}

void yellow()
{
    printf("\033[1;93m");
}

void reset()
{
    printf("\033[0m");
}

void showMenu(int sd);
void chooseOption(int sd);

int option, currUserID;

// attempt student login
void attemptStudentLogin(int sd)
{
    bool result;
    struct student currUser;
    ssize_t write_res;
    ssize_t read_res;

    printf("Student ID : ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;

    printf("Password : ");
    char *pass = getpass("");
    strcpy(currUser.password, pass);

    getchar();

    write_res = write(sd, &option, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    write_res = write(sd, &currUser, sizeof(struct student));
    if (write_res < 0 || write_res != sizeof(struct student))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("\nUnable to Login!\nEither your password didn't match or your account is deleted.\n\n");
        reset();
        chooseOption(sd);
    }
    else
    {
        green();
        printf("\nSuccessfully logged in!\n\n");
        reset();
    }
    return;
}

// attempt faculty login
void attemptFacultyLogin(int sd)
{
    bool result;
    struct faculty currUser;
    ssize_t write_res;
    ssize_t read_res;

    printf("Faculty ID : ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;

    printf("Password : ");
    char *pass = getpass("");
    strcpy(currUser.password, pass);

    getchar();

    write_res = write(sd, &option, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    write_res = write(sd, &currUser, sizeof(struct faculty));
    if (write_res < 0 || write_res != sizeof(struct faculty))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read failed");
        exit(EXIT_FAILURE); // Exit or handle the error as required
    }

    if (!result)
    {
        red();
        printf("\nUnable to Login!\nEither your password didn't match or your account is deleted.\n");
        reset();
        chooseOption(sd);
    }
    else
    {
        green();
        printf("\nSuccessfully logged in!\n\n");
        reset();
    }
    return;
}

// attempt admin login
void attemptAdminLogin(int sd)
{
    bool result;
    struct admin currUser;
    ssize_t write_res;
    ssize_t read_res;

    printf("Admin ID : ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;
    printf("Password : ");
    char *pass = getpass("");
    strcpy(currUser.password, pass);

    write_res = write(sd, &option, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    write_res = write(sd, &currUser, sizeof(struct admin));
    if (write_res < 0 || write_res != sizeof(struct admin))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("\nUnable to Login!\nEither your password didn't match or your account is deleted.\n");
        reset();
        chooseOption(sd);
    }
    else
    {
        green();
        printf("\nSuccessfully logged in!\n\n");
        reset();
    }
    return;
}

// choose type of user
void chooseOption(int sd)
{
    printf("1 : Student Login\n");
    printf("2 : Faculty Login\n");
    printf("3 : Admin Login\n");
    printf("\nChoose an option : ");

    scanf("%d", &option);
    printf("Selected Option : %d\n", option);

    switch (option)
    {
    case 1:
        attemptStudentLogin(sd);
        break;
    case 2:
        attemptFacultyLogin(sd);
        break;
    case 3:
        attemptAdminLogin(sd);
        break;
    default:
        red();
        printf("Invalid option!\n\n");
        reset();
        chooseOption(sd);
        break;
    }
    return;
}

// student view all courses
void viewAllCourses(int sd)
{
    int select = 1;
    int count;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write failed");
        exit(EXIT_FAILURE);
    }

    struct course foundCourse;

    read_res = read(sd, &count, sizeof(int));
    if (read_res < 0)
    {
        perror("read failed");
        exit(EXIT_FAILURE);
    }
    yellow();
    printf("\nTotal available courses are %d.\n", count);
    for (int i = 0; i < count; i++)
    {
        read_res = read(sd, &foundCourse, sizeof(struct course));
        if (read_res < 0)
        {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        printf("\nCourse ID: %d", foundCourse.courseID);
        printf("\nCourse Name: %s", foundCourse.name);
        printf("\nAvailable Seats: %d", foundCourse.available_seats);
        printf("\n");
    }
    printf("\n");

    reset();

    showMenu(sd);
}

// student enroll into a new course
void enrollCourse(int sd)
{
    int select = 2;
    bool result;
    int available_seats = 0;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct enrollment enroll;

    printf("Enter your student ID : ");
    scanf("%d", &enroll.studentID);
    printf("Enter Course ID to enroll : ");
    scanf("%d", &enroll.courseID);

    write_res = write(sd, &enroll, sizeof(struct enrollment));
    if (write_res < 0 || write_res != sizeof(struct enrollment))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &available_seats, sizeof(int));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    printf("\nNumber of available seats: %d\n", available_seats);

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (result == true)
    {
        green();
        printf("Successfully enrolled in course!\n\n");
        reset();
    }
    else
    {
        red();
        printf("Unable to enroll!\n\n");
        reset();
    }

    showMenu(sd);
}

// student unenroll from a course
void dropCourse(int sd)
{
    int select = 3;
    ssize_t write_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct enrollment dropEnroll;

    printf("Enter your student ID : ");
    scanf("%d", &dropEnroll.studentID);

    printf("Enter the course ID you want to drop : ");
    scanf("%d", &dropEnroll.courseID);

    write_res = write(sd, &dropEnroll, sizeof(struct enrollment));
    if (write_res < 0 || write_res != sizeof(struct enrollment))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }
    green();
    printf("\nUnenrolled successfully.\n\n");
    reset();

    showMenu(sd);
}

// student view all enrolled courses
void viewEnrolledCourses(int sd)
{
    int select = 4;
    int count = 0;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct enrollment searchedEnrollment;
    int studentID;
    printf("Enter your Student ID: ");
    scanf("%d", &studentID);

    write_res = write(sd, &studentID, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &count, sizeof(int));
    if (read_res < 0 || read_res != sizeof(int))
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    yellow();
    printf("\nYou are currently enrolled in %d courses.\n", count);
    for (int i = 0; i < count; i++)
    {
        read(sd, &searchedEnrollment, sizeof(struct enrollment));
        printf("\nCourse ID: %d", searchedEnrollment.courseID);
        printf("\n");
    }
    printf("\n");
    reset();

    showMenu(sd);
}

// student change password
void changeStudentPassword(int sd)
{
    int select = 5;
    bool result;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct student modifyStudent;
    printf("Enter the Student User ID to change password : ");
    scanf("%d", &modifyStudent.userID);

    printf("New Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(modifyStudent.password, pass);

    write_res = write(sd, &modifyStudent, sizeof(struct student));
    if (write_res < 0 || write_res != sizeof(struct student))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0 || read_res != sizeof(result))
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("Error changing the password,please re-check the User ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully changed the password!\n\n");
        reset();
    }
    showMenu(sd);
    return;
}

// faculty view all offered courses
void viewOfferedCourses(int sd)
{
    int select = 1;
    int count;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    int facultyID;
    printf("Enter your faculty ID: ");
    scanf("%d", &facultyID);
    printf("Entered Faculty ID : %d\n\n", facultyID);

    write_res = write(sd, &facultyID, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &count, sizeof(int));
    if (read_res < 0 || read_res != sizeof(int))
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    yellow();
    printf("You are currently offering %d courses.\n", count);

    struct course searchedCourse;

    for (int i = 0; i < count; i++)
    {
        read(sd, &searchedCourse, sizeof(struct course));
        printf("\nCourse ID: %d", searchedCourse.courseID);
        printf("\nCourse Name: %s", searchedCourse.name);
        printf("\nTotal seats: %d", searchedCourse.seats);
        printf("\n");
    }

    printf("\n");
    reset();

    showMenu(sd);
}

// faculty add new course
void addNewCourse(int sd)
{
    int select = 2;
    bool result;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct course addCourse;
    printf("Enter the Course ID : ");
    scanf("%d", &addCourse.courseID);
    printf("Enter name of the course to add : ");
    scanf(" %[^\n]", addCourse.name);
    printf("Enter your faculty ID : ");
    scanf("%d", &addCourse.facultyID);
    int seats;
    printf("Enter number of seats : ");
    scanf("%d", &seats);
    addCourse.seats = seats;
    addCourse.available_seats = seats;

    write_res = write(sd, &addCourse, sizeof(struct course));
    if (write_res < 0 || write_res != sizeof(struct course))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("Error in adding course,please re-check if you entered details correctly!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully added the course!\n\n");
        reset();
    }

    showMenu(sd);
    return;
}

// faculty remove offered course
void removeOfferedCourse(int sd)
{
    int select = 3;
    ssize_t write_res;
    int courseID;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    printf("Enter the course ID to delete: ");
    scanf("%d", &courseID);
    printf("Entered Course ID : %d\n\n", courseID);

    write_res = write(sd, &courseID, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    green();
    printf("Course deleted successfully.\n\n");
    reset();
    showMenu(sd);
}

// faculty update course details
void updateCourseDetails(int sd)
{
    int select = 4;
    bool result;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write select to server failed");
        exit(EXIT_FAILURE);
    }

    struct course modifyCourse;

    printf("Enter the Course ID to be modified : ");
    scanf("%d", &modifyCourse.courseID);
    printf("Enter Updated Name of the Course : ");
    scanf(" %[^\n]", modifyCourse.name);
    printf("Enter new number of available seats : ");
    scanf("%d", &modifyCourse.seats);

    write_res = write(sd, &modifyCourse, sizeof(struct course));
    if (write_res < 0 || write_res != sizeof(struct course))
    {
        perror("write course details to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("Error modifying the course details,please re-check the course ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully modified the course details!\n\n");
        reset();
    }

    showMenu(sd);
    return;
}

// faculty change password
void changeFacultyPassword(int sd)
{
    int select = 5;
    bool result;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct faculty modifyFaculty;
    printf("Enter the Faculty User ID to be modified : ");
    scanf("%d", &modifyFaculty.userID);

    printf("New Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(modifyFaculty.password, pass);

    write_res = write(sd, &modifyFaculty, sizeof(struct faculty));
    if (write_res < 0 || write_res != sizeof(struct faculty))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0 || read_res != sizeof(result))
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("Error changing the faculty password,please re-check the User ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully changed the password!\n\n");
        reset();
    }
    showMenu(sd);
    return;
}

// admin add student
void addStudent(int sd)
{
    int select = 1;
    bool result;
    ssize_t write_res, read_res;
    char rdBuff[1000];
    bzero(rdBuff, sizeof(rdBuff));

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct student newStudent;
    printf("Name of Student: ");
    scanf(" %[^\n]", newStudent.name);
    printf("Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(newStudent.password, pass);

    write_res = write(sd, &newStudent, sizeof(struct student));
    if (write_res < 0 || write_res != sizeof(struct student))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }
    read_res = read(sd, rdBuff, sizeof(rdBuff));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0 || read_res != sizeof(result))
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("Error adding the student record!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully added the student.\n\n");
        reset();
    }
    printf("%s\n", rdBuff);
    showMenu(sd);
    return;
}

// admin search student
void viewStudent(int sd)
{
    int select = 2;
    int len;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct student searchStudent;
    int userID;
    printf("Enter UserID: ");
    scanf("%d", &userID);

    write_res = write(sd, &userID, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    len = read(sd, &searchStudent, sizeof(struct student));

    if (len == 0)
    {
        printf("\nPlease re-check the User ID!\n\n");
    }
    else
    {
        yellow();
        printf("\nUser ID : %d\n", searchStudent.userID);
        printf("Name : %s\n", searchStudent.name);
        printf("Status : %s\n\n", searchStudent.status);
        reset();
    }

    showMenu(sd);
}

// admin add faculty
void addFaculty(int sd)
{
    int select = 3;
    ssize_t write_res, read_res;
    bool result;
    char rdBuff[1000];
    bzero(rdBuff, sizeof(rdBuff));

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct faculty newFaculty;
    printf("Name of Faculty: ");
    scanf(" %[^\n]", newFaculty.name);
    printf("Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(newFaculty.password, pass);

    write_res = write(sd, &newFaculty, sizeof(struct faculty));
    if (write_res < 0 || write_res != sizeof(struct faculty))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }
    read_res = read(sd, rdBuff, sizeof(rdBuff));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("Error adding the faculty record!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully added the faculty.\n\n");
        reset();
    }
    printf("%s\n", rdBuff);
    showMenu(sd);
    return;
}

// admin search faculty
void viewFaculty(int sd)
{
    int select = 4;
    ssize_t write_res, read_res;
    int len;
    bool result;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct faculty searchFaculty;
    int userID;
    printf("Enter UserID: ");
    scanf("%d", &userID);

    write_res = write(sd, &userID, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    len = read(sd, &searchFaculty, sizeof(struct faculty));

    if (len == 0)
    {
        printf("Please re-check the User ID!\n\n");
    }
    else
    {
        yellow();
        printf("\nUser ID : %d\n", searchFaculty.userID);
        printf("Name : %s\n\n", searchFaculty.name);
        reset();
    }
    showMenu(sd);
}

// admin activate student
void activateStudent(int sd)
{
    int select = 5;
    bool result;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    struct student activateStudent1;
    printf("Enter the Student User ID to be activated : ");
    scanf("%d", &activateStudent1.userID);

    write_res = write(sd, &activateStudent1, sizeof(struct student));
    if (write_res < 0 || write_res != sizeof(struct student))
    {
        perror("write to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }

    if (!result)
    {
        red();
        printf("\nError activating student,please re-check the User ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully activated the student!\n\n");
        reset();
    }

    showMenu(sd);
    return;
}

// admin block student
void blockStudent(int sd)
{
    int select = 6;
    bool result;
    ssize_t write_res, read_res;

    write_res = write(sd, &select, sizeof(int));
    if (write_res < 0 || write_res != sizeof(int))
    {
        perror("write select to server failed");
        exit(EXIT_FAILURE);
    }

    struct student blockStudent;
    printf("Enter the Student User ID to be blocked : ");
    scanf("%d", &blockStudent.userID);

    write_res = write(sd, &blockStudent, sizeof(struct student));
    if (write_res < 0 || write_res != sizeof(struct student))
    {
        perror("write student data to server failed");
        exit(EXIT_FAILURE);
    }

    read_res = read(sd, &result, sizeof(result));
    if (read_res < 0)
    {
        perror("read from server failed");
        exit(EXIT_FAILURE);
    }
    if (!result)
    {
        red();
        printf("\nError blocking student,please re-check the User ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully blocked the student!\n\n");
        reset();
    }
    showMenu(sd);
    return;
}

// admin modify student details
void modifyStudent(int sd)
{
    int select = 7;
    bool result;

    write(sd, &select, sizeof(int));

    struct student modifyStudent;
    printf("Enter the Student User ID to be modified : ");
    scanf("%d", &modifyStudent.userID);

    printf("New Name of the Student : ");
    scanf(" %[^\n]", modifyStudent.name);
    printf("New Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(modifyStudent.password, pass);

    write(sd, &modifyStudent, sizeof(struct student));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        red();
        printf("Error modifying the student details,please re-check the User ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccessfully modified the student details!\n\n");
        reset();
    }
    showMenu(sd);
    return;
}

// admin modify faculty details
void modifyFaculty(int sd)
{
    int select = 8;
    bool result;

    write(sd, &select, sizeof(int));

    struct faculty modifyFaculty;
    printf("Enter the Faculty User ID to be modified : ");
    scanf("%d", &modifyFaculty.userID);

    printf("New Name of the Faculty : ");
    scanf(" %[^\n]", modifyFaculty.name);
    printf("New Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(modifyFaculty.password, pass);

    write(sd, &modifyFaculty, sizeof(struct faculty));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        red();
        printf("Error modifying the faculty details,please re-check the User ID!\n\n");
        reset();
    }
    else
    {
        green();
        printf("\nSuccesfully modified the faculty details!\n\n");
        reset();
    }
    showMenu(sd);
    return;
}

void showMenu(int sd)
{
    int select;
    // student
    if (option == 1)
    {
        printf("..........Welcome to Student Menu..........\n\n");
        printf("1 : View All Courses\n");
        printf("2 : Enroll into a course\n");
        printf("3 : Drop course\n");
        printf("4 : View enrolled course details\n");
        printf("5 : Change Password\n");
        printf("6 : Exit\n");

        printf("Select an option: ");
        scanf("%d", &select);
        printf("Selected Option : %d\n", select);

        switch (select)
        {
        case 1:
            viewAllCourses(sd);
            break;
        case 2:
            enrollCourse(sd);
            break;
        case 3:
            dropCourse(sd);
            break;
        case 4:
            viewEnrolledCourses(sd);
            break;
        case 5:
            changeStudentPassword(sd);
            break;
        case 6:
            write(sd, &select, sizeof(int));
            printf("Bye Bye!\n");
            exit(0);
        default:
            printf("Invalid option!\n\n");
            showMenu(sd);
            break;
        }
    }
    // faculty
    else if (option == 2)
    {
        printf("..........Welcome to Faculty Menu..........\n\n");
        printf("1 : View offered courses\n");
        printf("2 : Add new course\n");
        printf("3 : Remove currently offered course\n");
        printf("4 : Update course details\n");
        printf("5 : Change Password\n");
        printf("6 : Exit\n");

        printf("Select an option: ");
        scanf("%d", &select);
        printf("Selected Option : %d\n", select);

        switch (select)
        {
        case 1:
            viewOfferedCourses(sd);
            break;
        case 2:
            addNewCourse(sd);
            break;
        case 3:
            removeOfferedCourse(sd);
            break;
        case 4:
            updateCourseDetails(sd);
            break;
        case 5:
            changeFacultyPassword(sd);
            break;
        case 6:
            write(sd, &select, sizeof(int));
            printf("Bye Bye!\n");
            exit(0);
        default:
            printf("Invalid option!\n\n");
            showMenu(sd);
            break;
        }
    }
    // admin
    else if (option == 3)
    {
        printf("..........Welcome to Admin Menu..........\n\n");
        printf("1 : Add Student\n");
        printf("2 : View Student Details\n");
        printf("3 : Add Faculty\n");
        printf("4 : View Faculty Detials\n");
        printf("5 : Activate Student\n");
        printf("6 : Block Student\n");
        printf("7 : Modify Student Details\n");
        printf("8 : Modify Faculty Details\n");
        printf("9 : Exit\n");

        printf("Select an option: ");
        scanf("%d", &select);
        printf("Selected Option : %d\n", select);

        switch (select)
        {
        case 1:
            addStudent(sd);
            break;
        case 2:
            viewStudent(sd);
            break;
        case 3:
            addFaculty(sd);
            break;
        case 4:
            viewFaculty(sd);
            break;
        case 5:
            activateStudent(sd);
            break;
        case 6:
            blockStudent(sd);
            break;
        case 7:
            modifyStudent(sd);
            break;
        case 8:
            modifyFaculty(sd);
            break;
        case 9:
            write(sd, &select, sizeof(int));
            printf("Bye Bye!\n");
            exit(0);
        default:
            printf("Invalid option!\n\n");
            showMenu(sd);
            break;
        }
    }
}

int main()
{
    system("clear");
    printf("..........Welcome to Academia..........\n\n");

    struct sockaddr_in server;
    int sd, msgLength;
    char buff[50];
    char result;

    sd = socket(AF_INET, SOCK_STREAM, 0);

    if (sd == -1)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5555);

    if (connect(sd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connection failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // show initial login menu
    chooseOption(sd);

    system("clear");

    // show menu after login
    showMenu(sd);

    if (close(sd) < 0)
    {
        perror("closing socket failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}