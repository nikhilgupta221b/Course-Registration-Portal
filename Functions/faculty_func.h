#ifndef FACULTY_USER
#define FACULTY_USER

struct faculty searchFacultyRecord(int ID)
{
        int i = ID;
        struct faculty currUser;
        int fd = open("/home/nikhil/Academia/Database/Faculty.data", O_RDONLY, 0744);

        int fl1;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct faculty);
        lock.l_len = sizeof(struct faculty);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(struct faculty), SEEK_SET);
        read(fd, &currUser, sizeof(struct faculty));

        lock.l_type = F_UNLCK;
        printf("Press Enter to relese Lock from View Details\n");
        getchar();

        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return currUser;
}

bool checkFaculty(struct faculty currUser)
{
        int i = currUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Faculty.data", O_RDONLY, 0744);
        bool result;
        struct faculty temp;

        int fl1;
        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct faculty);
        lock.l_len = sizeof(struct faculty);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i) * sizeof(struct faculty), SEEK_SET);
        read(fd, &temp, sizeof(struct faculty));
        if (!strcmp(temp.password, currUser.password))
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}

bool addFaculty(struct faculty record, int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Faculty.data", O_RDWR, 0744);
        struct faculty lastFaculty;
        int flag = 0;

        if (fd == -1 && errno == ENOENT)
        {
                fd = open("/home/nikhil/Academia/Database/Faculty.data", O_RDWR | O_CREAT | O_APPEND, 0744);
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
        lock.l_start = (-1) * sizeof(struct faculty);
        lock.l_len = sizeof(struct faculty);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (-1) * sizeof(struct faculty), SEEK_END);

        read(fd, &lastFaculty, sizeof(struct faculty));

        if (flag == 0)
        {
                record.userID = lastFaculty.userID + 1;
        }
        printf("User ID = %d\n", record.userID);
        sprintf(wrBuff, "%s%d\n", "User ID of your new Account is ", record.userID);

        int j = write(fd, &record, sizeof(struct faculty));
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

bool updateFacultyDetails(struct faculty modUser)
{
        int i = modUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Faculty.data", O_RDWR, 0744);
        bool result = false;

        int fl1;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct faculty);
        lock.l_len = sizeof(struct faculty);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        struct faculty currUser;
        lseek(fd, (i) * sizeof(struct faculty), SEEK_SET);
        read(fd, &currUser, sizeof(struct faculty));

        strcpy(currUser.name, modUser.name);
        strcpy(currUser.password, modUser.password);

        lseek(fd,(-1)*sizeof(struct faculty), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(struct faculty));
        if (j != 0)
                result = true;
        else
                result = false;
        
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return result;
}

bool changeFacultyPass(struct faculty modUser)
{
        int i = modUser.userID;
        int fd = open("/home/nikhil/Academia/Database/Faculty.data", O_RDWR, 0744);
        bool result = false;

        int fl1;
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i) * sizeof(struct faculty);
        lock.l_len = sizeof(struct faculty);
        lock.l_pid = getpid();

        fl1 = fcntl(fd, F_SETLKW, &lock);

        struct faculty currUser;
        lseek(fd, (i) * sizeof(struct faculty), SEEK_SET);
        read(fd, &currUser, sizeof(struct faculty));

        strcpy(currUser.password, modUser.password);

        lseek(fd,(-1)*sizeof(struct faculty), SEEK_CUR);
        int j = write(fd, &currUser, sizeof(struct faculty));
        if (j != 0)
                result = true;
        else
                result = false;
        
        lock.l_type=F_UNLCK;
        fcntl(fd,F_SETLK,&lock);

        close(fd);
        return result;
}

#endif