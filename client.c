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

void showMenu(int sd);
void chooseOption(int sd);

int option, currUserID;

// Attempt Student Login
void attemptStudentLogin(int sd)
{
    bool result;
    struct student currUser;

    printf("Student ID : ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;

    printf("Password : ");
    char *pass = getpass("");
    strcpy(currUser.password, pass);

    getchar();

    // send selected option & data to server
    write(sd, &option, sizeof(int));
    write(sd, &currUser, sizeof(struct student));

    // read response of server
    read(sd, &result, sizeof(result));

    if (!result)
    {
        printf("Invalid login!\nEither your Password didn't match OR your Account is Deleted\n");
        chooseOption(sd);
    }
    else
    {
        printf("Succesfully logged in!\n\n");
    }
    return;
}

// Attempt Faculty Login
void attemptFacultyLogin(int sd)
{
    bool result;
    struct faculty currUser;

    printf("Faculty ID : ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;

    printf("Password : ");
    char *pass = getpass("");
    strcpy(currUser.password, pass);

    getchar();

    // send selected option & data to server
    write(sd, &option, sizeof(int));
    write(sd, &currUser, sizeof(struct faculty));

    // read response of server
    read(sd, &result, sizeof(result));

    if (!result)
    {
        printf("Invalid login!\nEither your Password didn't match OR your Account is Deleted\n");
        chooseOption(sd);
    }
    else
    {
        printf("Succesfully logged in!\n\n");
    }
    return;
}

// Login Attempt Admin
void attemptAdminLogin(int sd)
{
    bool result;
    struct admin currUser;
    printf("Admin ID : ");
    scanf("%d", &currUser.userID);
    currUserID = currUser.userID;
    printf("Password : ");
    char *pass = getpass("");
    strcpy(currUser.password, pass);

    write(sd, &option, sizeof(int));
    write(sd, &currUser, sizeof(struct admin));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        printf("Invalid login!\nEither your Password didn't match OR your Account is Deleted\n");
        chooseOption(sd);
    }
    else
    {
        printf("Succesfully logged in!\n\n");
    }
    return;
}

// Choose Type of User
void chooseOption(int sd)
{
    printf("1 : Student Login\n");
    printf("2 : Faculty Login\n");
    printf("3 : Admin Login\n");
    printf("Choose an option : ");

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
        printf("Invalid option!\n\n");
        chooseOption(sd);
        break;
    }
    return;
}

// add student
void addStudent(int sd)
{
    int select = 1;
    bool result;
    char rdBuff[1000];
    bzero(rdBuff, sizeof(rdBuff));

    write(sd, &select, sizeof(int));

    struct student newStudent;
    printf("Name of Student: ");
    scanf(" %[^\n]", newStudent.name);
    printf("Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(newStudent.password, pass);

    write(sd, &newStudent, sizeof(struct student));
    read(sd, rdBuff, sizeof(rdBuff));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        printf("Error adding the student record!\n\n");
    }
    else
    {
        printf("Succesfully added the student.\n\n");
    }
    printf("%s\n", rdBuff);
    showMenu(sd);
    return;
}

// search student
void viewStudent(int sd)
{
    int select = 2;
    int len;
    bool result;
    write(sd, &select, sizeof(int));

    struct student searchStudent;
    int userID;
    printf("Enter UserID: ");
    scanf("%d", &userID);
    write(sd, &userID, sizeof(int));

    len = read(sd, &searchStudent, sizeof(struct student));

    if (len == 0)
    {
        printf("Please re-check the User ID!\n\n");
    }
    else
    {
        printf("User ID : %d\n", searchStudent.userID);
        printf("Name : %s\n", searchStudent.name);
        printf("Status : %s\n\n", searchStudent.status);
    }
    showMenu(sd);
}

// add faculty
void addFaculty(int sd)
{
    int select = 3;
    bool result;
    char rdBuff[1000];
    bzero(rdBuff, sizeof(rdBuff));

    write(sd, &select, sizeof(int));

    struct faculty newFaculty;
    printf("Name of Faculty: ");
    scanf(" %[^\n]", newFaculty.name);
    printf("Password(max 10 characters) : ");
    char *pass = getpass("");
    strcpy(newFaculty.password, pass);

    write(sd, &newFaculty, sizeof(struct faculty));
    read(sd, rdBuff, sizeof(rdBuff));

    read(sd, &result, sizeof(result));

    if (!result)
    {
        printf("Error adding the faculty record!\n\n");
    }
    else
    {
        printf("Succesfully added the faculty.\n\n");
    }
    printf("%s\n", rdBuff);
    showMenu(sd);
    return;
}

// search faculty
void viewFaculty(int sd)
{
    int select = 4;
    int len;
    bool result;
    write(sd, &select, sizeof(int));

    struct faculty searchFaculty;
    int userID;
    printf("Enter UserID: ");
    scanf("%d", &userID);
    write(sd, &userID, sizeof(int));

    len = read(sd, &searchFaculty, sizeof(struct faculty));

    if (len == 0)
    {
        printf("Please re-check the User ID!\n\n");
    }
    else
    {
        printf("User ID : %d\n", searchFaculty.userID);
        printf("Name : %s\n\n", searchFaculty.name);
    }
    showMenu(sd);
}

// activate student
void activateStudent(int sd)
{
    int select = 5;
    bool result;

    write(sd, &select, sizeof(int));

    struct student activateStudent1;
    printf("Enter the Student User ID to be activated : ");
    scanf("%d", &activateStudent1.userID);

    write(sd, &activateStudent1, sizeof(struct student));

    read(sd, &result, sizeof(result));
    if (!result)
    {
        printf("Error activating student,please re-check the User ID!\n\n");
    }
    else
    {
        printf("Succesfully activated the student!\n\n");
    }

    showMenu(sd);
    return;
}

// block student
void blockStudent(int sd)
{
    int select = 6;
    bool result;

    write(sd, &select, sizeof(int));

    struct student blockStudent;
    printf("Enter the Student User ID to be blocked : ");
    scanf("%d", &blockStudent.userID);

    write(sd, &blockStudent, sizeof(struct student));

    read(sd, &result, sizeof(result));
    if (!result)
    {
        printf("Error blocking student,please re-check the User ID!\n\n");
    }
    else
    {
        printf("Succesfully blocked the student!\n\n");
    }
    showMenu(sd);
    return;
}

// modify student details
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
        printf("Error modifying the student details,please re-check the User ID!\n\n");
    }
    else
    {
        printf("Succesfully modified the student details!\n\n");
    }
    showMenu(sd);
    return;
}

// modify faculty details
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
        printf("Error modifying the faculty details,please re-check the User ID!\n\n");
    }
    else
    {
        printf("Succesfully modified the faculty details!\n\n");
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
            // viewCourses(sd);
            break;
        case 2:
            // enrollCourse(sd);
            break;
        case 3:
            // dropCourse(sd);
            break;
        case 4:
            // enrolledCourse(sd);
            break;
        case 5:
            // changePassword(sd);
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
            // viewCourses(sd);
            break;
        case 2:
            // enrollCourse(sd);
            break;
        case 3:
            // dropCourse(sd);
            break;
        case 4:
            // enrolledCourse(sd);
            break;
        case 5:
            // changePassword(sd);
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
    printf("Welcome to Academia\n\n");

    struct sockaddr_in server;
    int sd, msgLength;
    char buff[50];
    char result;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(5555);

    connect(sd, (struct sockaddr *)&server, sizeof(server));

    // Show Intial Login Menu
    chooseOption(sd);

    // Showing menu after login
    showMenu(sd);

    close(sd);
    return 0;
}