#ifndef ENROLL_FUNC
#define ENROLL_FUNC

// student enroll into a course
bool enrollStudentCourse(struct enrollment record, int sd)
{
        bool result = false;
        ssize_t bytesWritten;

        // checking if seats are available
        int cid = record.courseID;
        int avaiSeats = availableSeats(cid);

        bytesWritten = write(sd, &avaiSeats, sizeof(int));
        if (bytesWritten != sizeof(int))
        {
                perror("Error writing available seats to client");
                return false;
        }

        // if seats available add entry
        if (avaiSeats > 0)
        {
                int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR | O_CREAT | O_APPEND, 0744);
                if (fd == -1)
                {
                        perror("Error opening Enroll.data");
                        return false;
                }

                struct flock lock;
                lock.l_type = F_WRLCK;
                lock.l_whence = SEEK_SET;
                lock.l_start = 0;
                lock.l_len = 0;

                if (fcntl(fd, F_SETLKW, &lock) == -1)
                {
                        perror("Error locking Enroll.data");
                        close(fd);
                        return false;
                }

                int j = write(fd, &record, sizeof(struct enrollment));

                reduceAvailableSeats(cid);

                lock.l_type = F_UNLCK;
                if (fcntl(fd, F_SETLK, &lock) == -1)
                {
                        perror("Error unlocking Enroll.data");
                }

                close(fd);
                result = true;
        }
        return result;
}

// student unenroll from a course
void deleteEnrollment(struct enrollment removeEnrollment, int sd)
{
        struct enrollment buffer;
        ssize_t bytesRead;

        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR, 0744);
        if (fd == -1)
        {
                perror("Error opening Enroll.data");
                return;
        }
        int tmp_fd = open("/home/nikhil/Academia/Database/Enroll.data.tmp", O_RDWR | O_CREAT, 0744);
        if (tmp_fd == -1)
        {
                perror("Error opening Enroll.data.tmp");
                close(fd);
                return;
        }

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data for write");
                close(fd);
                close(tmp_fd);
                return;
        }

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.studentID != removeEnrollment.studentID || buffer.courseID != removeEnrollment.courseID)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }

        remove("/home/nikhil/Academia/Database/Enroll.data");
        rename("/home/nikhil/Academia/Database/Enroll.data.tmp", "/home/nikhil/Academia/Database/Enroll.data");

        increaseAvailableSeats(removeEnrollment.courseID);

        close(fd);
        close(tmp_fd);
}

// student view enrolled courses
void viewEnrolledCourses(int studentID, int sd)
{
        struct enrollment buffer;
        ssize_t bytesRead;
        int sID = studentID;
        int count = 0;

        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDONLY, 0744);
        if (fd == -1)
        {
                perror("Error opening Enroll.data");
                return;
        }

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data for read");
                close(fd);
                return;
        }

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.studentID == sID)
                        count++;
        }

        // send this count to client
        write(sd, &count, sizeof(int));

        // reset file pointer to start of file
        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.studentID == sID)
                {
                        write(sd, &buffer, sizeof(struct enrollment));
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }

        close(fd);
}

// unenroll all students from a course deleted by faculty
void removeAllEnrollments(int cid)
{
        struct enrollment buffer;
        ssize_t bytesRead;

        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR, 0744);
        int tmp_fd = open("/home/nikhil/Academia/Database/Enroll.data.tmp", O_RDWR | O_CREAT, 0744);

        struct flock lock;
        memset(&lock, 0, sizeof(lock));
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;

        if (fcntl(fd, F_SETLKW, &lock) == -1)
        {
                perror("Error locking Enroll.data");
                close(fd);
                close(tmp_fd);
                return;
        }

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.courseID != cid)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                }
        }

        lock.l_type = F_UNLCK;
        if (fcntl(fd, F_SETLK, &lock) == -1)
        {
                perror("Error unlocking Enroll.data");
        }

        remove("/home/nikhil/Academia/Database/Enroll.data");
        rename("/home/nikhil/Academia/Database/Enroll.data.tmp", "/home/nikhil/Academia/Database/Enroll.data");
        close(fd);
        close(tmp_fd);
}

// unenroll last students if faculty reduce seats
void unenrollLastStudents(struct course currCourse)
{
        printf("\n\nUnenrolling students\n\n");
        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR);
        if (fd == -1)
        {
                perror("Error opening Enroll.data");
                return;
        }
        int tmp_fd = open("/home/nikhil/Academia/Database/Enroll.data.tmp", O_RDWR | O_CREAT, 0744);
        if (tmp_fd == -1)
        {
                perror("Error creating temporary file");
                close(fd);
                return;
        }
        struct enrollment buffer;
        ssize_t bytesRead;
        int remseats = currCourse.seats;
        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.courseID != currCourse.courseID)
                {
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                }
                else if (remseats > 0)
                {
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                        remseats--;
                }
        }
        close(fd);
        close(tmp_fd);

        remove("/home/nikhil/Academia/Database/Enroll.data");
        rename("/home/nikhil/Academia/Database/Enroll.data.tmp", "/home/nikhil/Academia/Database/Enroll.data");
}

#endif