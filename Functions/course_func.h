#ifndef COURSES
#define COURSES

void removeAllEnrollments(int);
void unenrollLastStudents(struct course);

// student view all courses
void viewAllCourses(int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR | O_CREAT, 0744);
        struct course buffer;
        ssize_t bytesRead;
        int count = 0;

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Course.data");
                close(fd);
                return;
        }

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                count++;
        }

        // send this count to client
        ssize_t bytesWritten = write(sd, &count, sizeof(int));
        if (bytesWritten == -1 || bytesWritten != sizeof(int))
        {
                perror("Error sending count to client");
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
                return;
        }

        // reset pointer back to start
        if (lseek(fd, 0, SEEK_SET) == -1)
        {
                perror("Error seeking in Course.data");
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
                return;
        }

        // send course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                write(sd, &buffer, sizeof(struct course));
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
}

// faculty view offered courses
void viewOfferedCourses(int facultyID, int sd)
{
        struct course buffer;
        ssize_t bytesRead;
        int count = 0;
        int fID = facultyID;

        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR | O_CREAT, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return;
        }

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return;
        }

        printf("Faculty ID to send courses : %d\n:", fID);

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.facultyID == fID)
                        count++;
        }

        // send this count to client
        write(sd, &count, sizeof(int));

        // go to beginning
        lseek(fd, 0, SEEK_SET);

        // send course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.facultyID == fID)
                {
                        write(sd, &buffer, sizeof(struct course));
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
        }
        close(fd);
}

// faculty add new course
bool addNewCourse(struct course record, int sd)
{
        bool result = false;
        int fd;
        fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR | O_CREAT | O_APPEND, 0744);
        if (fd == -1)
        {
                perror("Error opening Course.data");
                return false;
        }

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (lseek(fd, 0, SEEK_END) == -1)
        {
                perror("Error seeking in Course.data");
                lock.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &lock);
                close(fd);
                return false;
        }

        int j = write(fd, &record, sizeof(struct course));
        if (j != 0)
                result = true;
        else
                result = false;

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
}

// delete offered course
void deleteCourse(int courseID, int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        int tmp_fd = open("/home/nikhil/Academia/Database/Course.data.tmp", O_RDWR | O_CREAT, 0744);

        int cID = courseID;
        struct course buffer;
        ssize_t bytesRead;
        bool found = false;

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.courseID != cID)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct course));
                }
        }
        remove("/home/nikhil/Academia/Database/Course.data");
        rename("/home/nikhil/Academia/Database/Course.data.tmp", "/home/nikhil/Academia/Database/Course.data");
        removeAllEnrollments(cID);
        close(fd);
        close(tmp_fd);
}

// faculty update course details
bool updateCourseDetails(struct course modCourse)
{
        int cID = modCourse.courseID;
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening file for updating");
                return false;
        }

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0; // Lock the whole file

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file for updating");
                close(fd);
                return false;
        }

        bool result = false;
        ssize_t bytesRead;
        struct course currCourse;
        while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0)
        {
                if (currCourse.courseID == cID)
                {
                        int change_in_seats = currCourse.seats - modCourse.seats;
                        strcpy(currCourse.name, modCourse.name);
                        currCourse.seats = modCourse.seats;
                        currCourse.available_seats = currCourse.available_seats - change_in_seats;
                        if (currCourse.available_seats < 0)
                        {
                                currCourse.available_seats = 0;
                                unenrollLastStudents(modCourse);
                        }
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &currCourse, sizeof(struct course));
                        result = true;
                }
        }

        close(fd);
        return result;
}

// return count of available seats in a course
int availableSeats(int cid)
{
        struct course buffer;
        ssize_t bytesRead;

        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDONLY, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return -1;
        }

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return -1;
        }

        // reading one by one record
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.courseID == cid)
                {
                        return buffer.available_seats;
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
        }

        close(fd);
        return -1;
}

// decrease number of available seats in a course by one
void reduceAvailableSeats(int cid)
{
        ssize_t bytesRead;
        struct course currCourse;

        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return;
        }

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return;
        }

        while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0)
        {
                if (currCourse.courseID == cid)
                {
                        currCourse.available_seats = currCourse.available_seats - 1;
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &currCourse, sizeof(struct course));
                        break;
                }
        }
        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
        }

        close(fd);
}

// increase number of available seats in a course by one
void increaseAvailableSeats(int cid)
{
        ssize_t bytesRead;
        struct course currCourse;

        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening file");
                return;
        }

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking file");
                close(fd);
                return;
        }

        while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0)
        {
                if (currCourse.courseID == cid)
                {
                        currCourse.available_seats = currCourse.available_seats + 1;
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &currCourse, sizeof(struct course));
                        break;
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking file");
        }

        close(fd);
}

#endif