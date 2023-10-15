# Academia 🎓
### A Course Registration Portal

<br>

⚫ <b><i>Project Goal:</i></b>
<br><br>To develop a course registration system which is user-friendly and multi-functional. <p>

This project mainly deals with the concurrent operations in a safe and secure environment.
- Safe: Concurrency handling.
- Secure: Only authorized faculty and students can access and modify the data.

<br>

⚫ <b><i>Features of Academia:</i></b>
- The application can handle concurrent access and operations.
- Socket programming is used to implement client-server model.
- Storage/ Database: Unix File System. i.e., data is stored typically in files only.
- Password protected login system for both admins and customers.

<br>

⚫ <b><i>How to execute?:</i></b>
- Firstly add a new Admin using a file called "create_admin.c"
- Change the absolute path mentioned in the code.
- Now compile and run the Server:
```
cc server.c -o server.out
./server.out
```
- Compile and run the Client:
```
cc client.c -o client.out
./client.out
```
- Done!

<br>

⚫ <b><i>Output:</i></b>

<br>

<b>Starting Application -</b>
- Create an Admin account
# ![InitClient](./Outputs/01.CreateAdmin.png)
- Start Server
# ![InitClient](./Outputs/02.StartServer.png)
- Start Client
# ![InitClient](./Outputs/03.StartClient.png)

<br>

<b>Admin Functions -</b>
- Add student:
# ![InitClient](./Outputs/04.AddStudent.png)
- View student details:
# ![InitClient](./Outputs/05.ViewStudent.png)
- Block student:
# ![InitClient](./Outputs/06.BlockStudent.png)
- After blocking:
# ![InitClient](./Outputs/07.AfterBlocking.png)
- Modify Faculty Details:
# ![InitClient](./Outputs/08.UpdateFaculty.png)

<br>

<b>Faculty Functions -<b>
- Add a new course:
# ![InitClient](./Outputs/09.AddCourse.png)
- View offered courses:
# ![InitClient](./Outputs/10.ViewOfferedCourse.png)
- Modify course details:
# ![InitClient](./Outputs/11.ModifyCourse.png)
- After modifying course:
# ![InitClient](./Outputs/12.AfterModifying.png)

<br>

<b>Student Functions -</b>
- View all courses:
# ![InitClient](./Outputs/13.ViewAllCourses.png)
- Enroll in a course:
# ![InitClient](./Outputs/14.Enroll.png)
- View Enrolled Courses:
# ![InitClient](./Outputs/15.AfterEnroll.png)
- Unenroll from a course:
# ![InitClient](./Outputs/16.Unenroll.png)




