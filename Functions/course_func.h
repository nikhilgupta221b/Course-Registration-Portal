#ifndef COURSES
#define COURSES

void removeAllEnrollments(int);
void unenrollLastStudents(struct course);
// faculty add new course
bool addNewCourse(struct course record, int sd)
{

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
        while ((bytesRead = read(fd, &currCourse, sizeof(struct course))) > 0)
        {
                if (currCourse.courseID == cID)
                {
                        int change_in_seats = currCourse.seats - modCourse.seats;
                        strcpy(currCourse.name, modCourse.name);
                        currCourse.seats = modCourse.seats;
                        currCourse.available_seats = currCourse.available_seats - change_in_seats;
                        lseek(fd, -sizeof(struct course), SEEK_CUR);
                        write(fd, &currCourse, sizeof(struct course));
                        result = true;
                }
        }
        unenrollLastStudents(currCourse);
        close(fd);
        return result;
}

void viewOfferedCourses(int facultyID, int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDONLY, 0744);
        struct course buffer;
        ssize_t bytesRead;
        int fID = facultyID;
        // reading one by one record
        int count = 0;
        printf("Faculty ID to send courses : %d\n:", fID);
        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.facultyID == fID)
                        count++;
        }

        // send this count to client
        write(sd, &count, sizeof(int));

        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.facultyID == fID)
                {
                        write(sd, &buffer, sizeof(struct course));
                }
        }
        close(fd);
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

void viewAllCourses(int sd)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDONLY, 0744);
        struct course buffer;
        ssize_t bytesRead;
        // reading one by one record
        int count = 0;

        // count number of course offered by faculty
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                count++;
        }

        // send this count to client
        write(sd, &count, sizeof(int));

        lseek(fd, 0, SEEK_SET);

        // sending course details to client
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                write(sd, &buffer, sizeof(struct course));
        }
        close(fd);
}

int availableSeats(int cid)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDONLY, 0744);
        struct course buffer;
        ssize_t bytesRead;

        // reading one by one record
        while ((bytesRead = read(fd, &buffer, sizeof(struct course))) > 0)
        {
                if (buffer.courseID == cid)
                {
                        return buffer.available_seats;
                }
        }

        close(fd);
        return -1;
}

void reduceAvailableSeats(int cid)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        ssize_t bytesRead;
        struct course currCourse;

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
        close(fd);
}

void increaseAvailableSeats(int cid)
{
        int fd = open("/home/nikhil/Academia/Database/Course.data", O_RDWR, 0744);
        ssize_t bytesRead;
        struct course currCourse;

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
        close(fd);
}

#endif