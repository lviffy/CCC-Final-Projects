# Student Report Management System (SMS)

## Overview
The **Student Management System (SMS)** is a robust, console-based application developed in C. It is designed to streamline the administration of student records for educational institutions. The system provides a secure, role-based environment where Administrators, Staff, and Students can interact with the data according to their assigned privileges.

This project demonstrates core C programming concepts including file handling, structure manipulation, pointers, and modular logic flow, consolidated into a single, easy-to-manage source file.

---

## Features

### Secure Authentication
-   **Login System**: All users must authenticate with a username and password.
-   **Role-Based Access Control (RBAC)**:
    -   **Admin**: Complete control over the system.
    -   **Staff**: Operational access to manage students.
    -   **User**: Read-only access for viewing records.
-   **Auto-Recovery**: If the credentials file is missing, the system automatically generates a default admin account to prevent lockout.

### User Roles & Capabilities

#### 1. Administrator (`admin`)
The superuser of the system with unrestricted access.
-   **Add Student**: Create new student records with Roll Number, Name, and Marks.
-   **Display Students**: View a formatted list of all students.
-   **Search Student**: Find specific students by their unique Roll Number.
-   **Update Student**: Modify details (Name, Marks) of existing students.
-   **Delete Student**: Permanently remove student records from the database.

#### 2. Staff (`STAFF`)
Designed for teachers and administrative staff.
-   **Add Student**: Register new students.
-   **Display Students**: View class lists.
-   **Search Student**: Quickly look up student details.

#### 3. User (`USER`)
Designed for students or parents.
-   **Display Students**: View public student lists.
-   **Search Student**: Check specific records.

---

## Technical Details

### File Structure
The project is designed to be self-contained.
-   **`main.c`**: The core source code containing all logic (Authentication, Menus, CRUD Operations).
-   **`students.txt`**: A text-based database storing student records.
    -   *Format*: `RollNumber Name Marks`
-   **`credentials.txt`**: A text-based database for user authentication.
    -   *Format*: `Username Password Role`

### Data Structures
The system uses a custom `Student` structure to manage data in memory:
```c
typedef struct {
    int roll;       // Unique identifier
    char name[50];  // Student's full name
    float marks;    // Academic score
} Student;
```

---

## Installation & Setup

### Prerequisites
-   **C Compiler**: GCC (GNU Compiler Collection) is recommended.
-   **Terminal**: Any standard command-line interface (Bash, Zsh, CMD, PowerShell).

### Step-by-Step Guide

1.  **Download the Source Code**
    Save the `main.c` file to your desired directory.

2.  **Compile the Application**
    Open your terminal, navigate to the directory, and run:
    ```bash
    gcc main.c -o student_system
    ```
    *Note: `student_system` is the name of the output executable. You can name it whatever you like.*

3.  **Run the Application**
    Execute the program:
    ```bash
    ./student_system
    ```
    *(On Windows, run `student_system.exe`)*

---

## Usage Guide

### First-Time Login
When you run the application for the first time, it will detect that no users exist and create a default admin account:
-   **Username**: `admin`
-   **Password**: `admin123`

### Managing Users
To add more users (Staff or Standard Users), you must manually edit the `credentials.txt` file. Open it in any text editor and add lines in the following format:
```text
username password ROLE
```
**Examples:**
```text
john_doe secret123 STAFF
jane_smith pass456 USER
```
*Supported Roles: `admin`, `STAFF`, `USER` (Case sensitive for logic, but usually stored as uppercase in file).*

### Common Operations
1.  **Adding a Student**: Select Option 1 from Admin/Staff menu. Enter a unique Roll Number.
2.  **Updating a Student**: Select Option 4 (Admin only). Enter the Roll Number. If found, input the new Name and Marks.
3.  **Deleting a Student**: Select Option 5 (Admin only). Enter the Roll Number to remove the record.

---
