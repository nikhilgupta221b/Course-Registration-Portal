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

#include"./Structures/student.h"
#include"./Structures/faculty.h"
#include"./Structures/admin.h"

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
    close(sd);
    return 0;
}