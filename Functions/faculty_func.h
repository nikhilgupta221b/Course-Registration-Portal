#ifndef FACULTY_USER
#define FACULTY_USER

// search and show details of faculty
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

// faculty login verification
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

// add new faculty by admin
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

// update faculty details by admin
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

// chage faculty password
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

// faculty add new course
bool addNewCourse(struct course record, int sd){
        
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR | O_CREAT | O_APPEND, 0744);

        
        bool result;

        lseek(fd, 0, SEEK_END);
        int j = write(fd, &record, sizeof(struct course));
        if (j != 0)
                result = true;
        else
                result = false;

        close(fd);
        return result;
}

// update course details by faculty
bool updateCourseDetails(struct course modCourse)
{
        int cID = modCourse.courseID;
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        bool result = false;
        ssize_t bytesRead;
        struct course currCourse;

        while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0) {
                if (currCourse.courseID == cID){
                        int change_in_seats = currCourse.seats - modCourse.seats;
                        strcpy(currCourse.name, modCourse.name);
                        currCourse.seats = modCourse.seats;
                        currCourse.available_seats = currCourse.available_seats - change_in_seats;

                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &currCourse, sizeof(struct course));
                        return true;
                }
        }
        close(fd);
        return false;
}

void viewOfferedCourses(int facultyID, int sd){
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDONLY, 0744);
        struct course buffer;
        ssize_t bytesRead;
        int fID = facultyID;
        // reading one by one record
        int count=0;
        printf("Faculty ID to send courses : %d\n:", fID);
        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0) {
                if (buffer.facultyID == fID) count++;
        }

        // send this count to client
        write(sd,&count,sizeof(int));
        
        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0) {
                if (buffer.facultyID == fID){
                        write(sd,&buffer,sizeof(struct course));
                }
        }
        close(fd);
}

// delete offered course
void deleteCourse(int courseID, int sd){
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        int tmp_fd = open("/home/nikhil/Academia/Database/Course.data.tmp", O_RDWR | O_CREAT, 0744);
        
        int cID = courseID;
        struct course buffer;
        ssize_t bytesRead;
        bool found = false;

        

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0) {
                if (buffer.courseID != cID){
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd,&buffer,sizeof(struct course));
                }
        }
        remove("/home/nikhil/Academia/Database/Course.data");
        rename("/home/nikhil/Academia/Database/Course.data.tmp", "/home/nikhil/Academia/Database/Course.data");
        close(fd);
        close(tmp_fd);
}

#endif