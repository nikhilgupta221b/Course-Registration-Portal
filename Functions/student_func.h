#ifndef STUDENT_USER
#define STUDENT_USER

struct student searchStudentRecord(int ID)
{
        int i = ID;
        struct student currUser;
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDONLY, 0744);

        int fl1;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(struct student), SEEK_SET);
        read(fd, &currUser, sizeof(struct student));

        lock.l_type = F_UNLCK;

        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return currUser;
}

bool updateStudentDetails(struct student modUser)
{
        int i = modUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDWR, 0744);
        bool result = false;

        int fl1;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        struct student currUser;
        lseek(fd, (i) * sizeof(struct student), SEEK_SET);
        read(fd, &currUser, sizeof(struct student));

        strcpy(currUser.name, modUser.name);
        strcpy(currUser.password, modUser.password);

        lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(struct student));
        if (j != 0)
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}

bool changeStudentPass(struct student modUser)
{
        int i = modUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDWR, 0744);
        bool result = false;

        int fl1;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        struct student currUser;
        lseek(fd, (i) * sizeof(struct student), SEEK_SET);
        read(fd, &currUser, sizeof(struct student));

        strcpy(currUser.password, modUser.password);

        lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(struct student));
        if (j != 0)
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}

bool activateStudentStatus(struct student modUser)
{
        int i = modUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDWR, 0744);
        bool result = false;

        int fl1;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        struct student currUser;
        lseek(fd, (i) * sizeof(struct student), SEEK_SET);
        read(fd, &currUser, sizeof(struct student));

        strcpy(currUser.status, "ACTIVE");

        lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(struct student));
        if (j != 0)
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}
bool blockStudentStatus(struct student modUser)
{
        int i = modUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDWR, 0744);
        bool result = false;

        int fl1;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        struct student currUser;

        lseek(fd, (i) * sizeof(struct student), SEEK_SET);
        read(fd, &currUser, sizeof(struct student));

        strcpy(currUser.status, "BLOCKED");

        lseek(fd, (-1) * sizeof(struct student), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(struct student));
        if (j != 0)
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}

bool checkStudent(struct student currUser)
{
        int i = currUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDONLY, 0744);
        bool result;
        struct student temp;

        int fl1;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(struct student), SEEK_SET);
        read(fd, &temp, sizeof(struct student));
        if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}
bool addStudent(struct student record, int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Student.data", O_RDWR, 0744);
        struct student lastStudent;
        int flag = 0;

        if (fd == -1 && errno == ENOENT)
        {
                fd = open("/home/nikhil/Academia/Database/Student.data", O_RDWR | O_CREAT | O_APPEND, 0744);
                record.userID = 0;
                flag = 1;
        }

        bool result;

        int fl1;
        char wrBuff[1000];
        bzero(wrBuff, sizeof(wrBuff));

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = (-1) * sizeof(struct student);
        lock.l_len = sizeof(struct student);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (-1) * sizeof(struct student), SEEK_END);

        read(fd, &lastStudent, sizeof(struct student));

        if (flag == 0)
        {
                record.userID = lastStudent.userID + 1;
        }
        printf("User ID = %d\n", record.userID);
        sprintf(wrBuff, "%s%d\n", "User ID of your new Account is ", record.userID);
        strcpy(record.status, "ACTIVE");

        int j = write(fd, &record, sizeof(struct student));
        if (j != 0)
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        write(sd, wrBuff, sizeof(wrBuff));
        return result;
}
#endif