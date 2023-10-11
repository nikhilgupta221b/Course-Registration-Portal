#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "./Structures/admin.h"

int getNewAdmin()
{
    int fd = open("/home/nikhil/Academia/Database/Admin.data", O_RDONLY, 0744);
    if (fd == -1 && errno == ENOENT)
    {
        return 0;
    }
    struct admin record;
    lseek(fd, -sizeof(struct admin), SEEK_END);
    read(fd, &record, sizeof(struct admin));
    close(fd);
    return record.userID + 1;
}

int main()
{
    int choice = 0;
    struct admin newAdmin;
    newAdmin.userID = getNewAdmin();
    int fd = open("/home/nikhil/Academia/Database/Admin.data", O_RDWR | O_CREAT | O_APPEND, 0744);
    printf("Please enter the name of the admin: ");
    scanf(" %[^\n]", newAdmin.username);
    printf("Please enter the password(max 10 characters): ");
    scanf(" %[^\n]", newAdmin.password);
    printf("Your userID is : %d\n", newAdmin.userID);
    write(fd, &newAdmin, sizeof(struct admin));
    printf("Do you want to continue(0/1)? ");
    scanf("%d", &choice);
    while (choice)
    {
        newAdmin.userID = getNewAdmin();
        printf("Please enter the name of the admin: ");
        scanf(" %[^\n]", newAdmin.username);
        printf("Please enter the password(max 10 characters): ");
        scanf(" %[^\n]", newAdmin.password);
        printf("Your userID is : %d\n", newAdmin.userID);
        write(fd, &newAdmin, sizeof(struct admin));
        printf("Do you want to continue(0/1)? ");
        scanf("%d", &choice);
    }
    close(fd);

    return 0;
}
