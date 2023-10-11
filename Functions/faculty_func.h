#ifndef FACULTY_USER
#define FACULTY_USER

bool checkFaculty(struct faculty currUser)
{
        int i = currUser.userID;
        int fd = open("/home/nikhil/Academia/Data/Faculty.data", O_RDONLY, 0744);
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
        if (!strcmp(temp.password, currUser.password) && !strcmp(temp.status, "ACTIVE"))
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}

#endif