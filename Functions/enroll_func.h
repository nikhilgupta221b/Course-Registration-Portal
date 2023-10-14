#ifndef ENROLL_FUNC
#define ENROLL_FUNC

// delete all enrollments of students if course is deleted
void removeAllEnrollments(int cid)
{
        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR, 0744);
        int tmp_fd = open("/home/nikhil/Academia/Database/Enroll.data.tmp", O_RDWR | O_CREAT, 0744);

        struct enrollment buffer;
        ssize_t bytesRead;

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.courseID != cid)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                }
        }
        remove("/home/nikhil/Academia/Database/Enroll.data");
        rename("/home/nikhil/Academia/Database/Enroll.data.tmp", "/home/nikhil/Academia/Database/Enroll.data");
        close(fd);
        close(tmp_fd);
}

bool enrollStudentCourse(struct enrollment record, int sd)
{

        bool result = false;
        // checking if seats are available
        int cid = record.courseID;
        int avaiSeats = availableSeats(cid);
        write(sd, &avaiSeats, sizeof(int));
        // if seats available add entry
        if (avaiSeats > 0)
        {
                int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR | O_CREAT | O_APPEND, 0744);
                int j = write(fd, &record, sizeof(struct enrollment));
                reduceAvailableSeats(cid);
                close(fd);
                result = true;
        }
        return result;
}

void viewEnrolledCourses(int studentID, int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDONLY, 0744);
        struct enrollment buffer;
        ssize_t bytesRead;
        int sID = studentID;
        // reading one by one record
        int count = 0;

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.studentID == sID)
                        count++;
        }

        // send this count to client
        write(sd, &count, sizeof(int));

        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.studentID == sID)
                {
                        write(sd, &buffer, sizeof(struct enrollment));
                }
        }
        close(fd);
}

// delete an enrollment by student
void deleteEnrollment(struct enrollment removeEnrollment, int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR, 0744);
        int tmp_fd = open("/home/nikhil/Academia/Database/Enroll.data.tmp", O_RDWR | O_CREAT, 0744);

        struct enrollment buffer;
        ssize_t bytesRead;

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.studentID != removeEnrollment.studentID || buffer.courseID != removeEnrollment.courseID)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                }
        }
        remove("/home/nikhil/Academia/Database/Enroll.data");
        rename("/home/nikhil/Academia/Database/Enroll.data.tmp", "/home/nikhil/Academia/Database/Enroll.data");
        increaseAvailableSeats(removeEnrollment.courseID);
        close(fd);
        close(tmp_fd);
}

// unenroll last students
void unenrollLastStudents(struct course currCourse)
{
        int fd = open("/home/nikhil/Academia/Database/Enroll.data", O_RDWR, 0744);
        int tmp_fd = open("/home/nikhil/Academia/Database/Enroll.data.tmp", O_RDWR | O_CREAT, 0744);

        struct enrollment buffer;
        ssize_t bytesRead;
        int remseats = currCourse.seats;
        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct enrollment))) > 0)
        {
                if (buffer.courseID != currCourse.courseID)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                }
                else if (remseats > 0)
                {
                        lseek(tmp_fd, 0, SEEK_END);
                        write(tmp_fd, &buffer, sizeof(struct enrollment));
                        remseats--;
                }
        }
        remove("/home/nikhil/Academia/Database/Enroll.data");
        rename("/home/nikhil/Academia/Database/Enroll.data.tmp", "/home/nikhil/Academia/Database/Enroll.data");
        close(fd);
        close(tmp_fd);
}

#endif