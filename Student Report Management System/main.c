/*
 * Student Management System
 * -------------------------
 * A console-based application for managing student records.
 * Features:
 * - Role-based access control (Admin, Staff, User, Guest)
 * - CRUD operations for student records
 * - File-based persistence (students.txt, credentials.txt)
 * - Secure login and password management
 * - Input validation and error handling
 */

#include <stdio.h>
#include <string.h>

// File constants
#define STUDENT_FILE "students.txt"
#define CREDENTIALS_FILE "credentials.txt"

/*-----------------STRUCTURES & GLOBALS-----------------*/

// Structure to represent a student
typedef struct {
  int roll;      // Roll Number (Unique ID)
  char name[50]; // Student Name
  float marks;   // Student Marks
} Student;

// Global variables to store current session info
char currentRole[10]; // Stores the role of the logged-in user
char currentUser[50]; // Stores the username of the logged-in user

/*-----------------FUNCTION PROTOTYPES-----------------*/
int loginSystem();
void mainmenu();
void adminMenu();
void userMenu();
void staffMenu();
void guestMenu();
void addStudent();
void viewStudents();
void searchStudents();
void updateStudent();
void deleteStudent();
void changePassword();

/*-----------------MAIN FUNCTION-----------------*/
int main() {
  // Attempt login
  if (loginSystem()) {
    // If login successful, show the main menu
    mainmenu();
  } else {
    // If login failed, exit
    printf("\nAccess Denied. Exiting the system......\n");
  }
  return 0;
}

/*-----------------LOGIN SYSTEM-----------------*/
/*
 * Handles user authentication.
 * Checks credentials against credentials.txt.
 * Creates a default admin account if the file is missing.
 */
int loginSystem() {
  char username[50], password[50];
  char fileUser[50], filePass[50], fileRole[10];
  FILE *fp;

  printf("-----LOGIN-----\n");
  printf("Username: ");
  if (scanf("%49s", username) != 1)
    return 0;
  printf("Password: ");
  if (scanf("%49s", password) != 1)
    return 0;

  // Open credentials file
  fp = fopen(CREDENTIALS_FILE, "r");
  if (!fp) {
    printf("Credentials file not found. Creating default admin...\n");
    // Create default admin if file doesn't exist
    fp = fopen(CREDENTIALS_FILE, "w");
    if (!fp) {
      printf("Error: Unable to create %s\n", CREDENTIALS_FILE);
      return 0;
    }
    fprintf(fp, "admin admin123 admin\n");
    fclose(fp);
    fp = fopen(CREDENTIALS_FILE, "r");
  }

  if (!fp) {
    printf("Error: %s not found!\n", CREDENTIALS_FILE);
    return 0;
  }

  // Check credentials
  while (fscanf(fp, "%49s %49s %9s", fileUser, filePass, fileRole) == 3) {
    if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
      // Login successful
      strcpy(currentRole, fileRole);
      strcpy(currentUser, fileUser);
      fclose(fp);
      return 1;
    }
  }

  fclose(fp);
  printf("Invalid username or password.\n");
  return 0;
}

/*-----------------MENUS-----------------*/
/*
 * Routes the user to the appropriate menu based on their role.
 */
void mainmenu() {
  if (strcmp(currentRole, "admin") == 0) {
    adminMenu();
  } else if (strcmp(currentRole, "USER") == 0) {
    userMenu();
  } else if (strcmp(currentRole, "STAFF") == 0) {
    staffMenu();
  } else {
    guestMenu();
  }
}

/*
 * Admin Menu: Full access to all features.
 */
void adminMenu() {
  int choice;
  do {
    printf("\n-----ADMIN MENU-----\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Search Students\n");
    printf("4. Update Student\n");
    printf("5. Delete Student\n");
    printf("6. Change Password\n");
    printf("7. Logout\n");
    printf("Enter your choice: ");

    // Robust input handling
    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ; // Clear input buffer
      choice = 0;
    }

    switch (choice) {
    case 1:
      addStudent();
      break;
    case 2:
      viewStudents();
      break;
    case 3:
      searchStudents();
      break;
    case 4:
      updateStudent();
      break;
    case 5:
      deleteStudent();
      break;
    case 6:
      changePassword();
      break;
    case 7:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 7);
}

/*
 * User Menu: Read-only access.
 */
void userMenu() {
  int choice;
  do {
    printf("\n-----USER MENU-----\n");
    printf("1. Display Students\n");
    printf("2. Search Students\n");
    printf("3. Logout\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      choice = 0;
    }

    switch (choice) {
    case 1:
      viewStudents();
      break;
    case 2:
      searchStudents();
      break;
    case 3:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 3);
}

/*
 * Staff Menu: Can Add, Update, and View, but not Delete.
 */
void staffMenu() {
  int choice;
  do {
    printf("\n-----STAFF MENU-----\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Search Students\n");
    printf("4. Update Student\n");
    printf("5. Change Password\n");
    printf("6. Logout\n");
    printf("Enter your choice: ");

    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      choice = 0;
    }

    switch (choice) {
    case 1:
      addStudent();
      break;
    case 2:
      viewStudents();
      break;
    case 3:
      searchStudents();
      break;
    case 4:
      updateStudent();
      break;
    case 5:
      changePassword();
      break;
    case 6:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid choice. Please try again.\n");
    }
  } while (choice != 6);
}

/*
 * Guest Menu: Restricted access (read-only).
 */
void guestMenu() {
  int choice;
  do {
    printf("\n-----GUEST MENU-----\n");
    printf("1. Display Students\n");
    printf("2. Search Students\n");
    printf("3. Logout\n");
    printf("Enter your choice: ");

    // Robust input handling
    if (scanf("%d", &choice) != 1) {
      int c;
      while ((c = getchar()) != '\n' && c != EOF)
        ;
      printf("Invalid input. Please enter a number.\n");
      continue;
    }

    switch (choice) {
    case 1:
      viewStudents();
      break;
    case 2:
      searchStudents();
      break;
    case 3:
      printf("Logging out...\n");
      break;
    default:
      printf("Invalid choice. Try again.\n");
    }
  } while (choice != 3);
}

/*-----------------STUDENT OPERATIONS-----------------*/

/*
 * Adds a new student record to the file.
 * Validates input (non-empty name, marks 0-100).
 */
void addStudent() {
  Student s;
  FILE *fp = fopen(STUDENT_FILE, "a");
  if (!fp) {
    printf("Error: Unable to open file.\n");
    return;
  }

  printf("Enter Roll Number: ");
  if (scanf("%d", &s.roll) != 1) {
    printf("Invalid input.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    fclose(fp);
    return;
  }

  printf("Enter Name: ");
  scanf(" %[^\n]s", s.name);
  if (strlen(s.name) == 0) {
    printf("Error: Name cannot be empty.\n");
    fclose(fp);
    return;
  }

  // Validate marks
  do {
    printf("Enter Marks (0-100): ");
    scanf("%f", &s.marks);
    if (s.marks < 0 || s.marks > 100) {
      printf("Invalid marks! Please enter between 0 and 100.\n");
    }
  } while (s.marks < 0 || s.marks > 100);

  // Save to file in CSV format
  fprintf(fp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
  fclose(fp);
  printf("Student added successfully.\n");
}

/*
 * Displays all student records.
 */
void viewStudents() {
  Student s;
  FILE *fp = fopen(STUDENT_FILE, "r");
  if (!fp) {
    printf("Error: Unable to open file.\n");
    return;
  }

  printf("\n%-10s %-30s %-10s\n", "Roll", "Name", "Marks");
  printf("--------------------------------------------------\n");
  // Read CSV format
  while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) == 3) {
    printf("%-10d %-30s %-10.2f\n", s.roll, s.name, s.marks);
  }
  fclose(fp);
}

/*
 * Searches for a student by Roll Number.
 */
void searchStudents() {
  int roll, found = 0;
  Student s;
  FILE *fp = fopen(STUDENT_FILE, "r");
  if (!fp) {
    printf("Error: Unable to open file.\n");
    return;
  }

  printf("Enter Roll Number to search: ");
  if (scanf("%d", &roll) != 1) {
    printf("Invalid input.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    fclose(fp);
    return;
  }

  while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) == 3) {
    if (s.roll == roll) {
      printf("\nStudent Found:\n");
      printf("Roll: %d\nName: %s\nMarks: %.2f\n", s.roll, s.name, s.marks);
      found = 1;
      break;
    }
  }
  if (!found) {
    printf("Student with Roll Number %d not found.\n", roll);
  }
  fclose(fp);
}

/*
 * Updates a student's details (Name, Marks).
 * Uses a temporary file to rewrite data.
 */
void updateStudent() {
  int roll, found = 0;
  Student s;
  FILE *fp = fopen(STUDENT_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");

  if (!fp || !temp) {
    printf("Error: Unable to open file.\n");
    if (fp)
      fclose(fp);
    if (temp)
      fclose(temp);
    return;
  }

  printf("Enter Roll Number to update: ");
  if (scanf("%d", &roll) != 1) {
    printf("Invalid input.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    if (fp)
      fclose(fp);
    if (temp)
      fclose(temp);
    return;
  }

  while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) == 3) {
    if (s.roll == roll) {
      printf("Enter New Name: ");
      scanf(" %[^\n]s", s.name);
      do {
        printf("Enter New Marks (0-100): ");
        scanf("%f", &s.marks);
        if (s.marks < 0 || s.marks > 100) {
          printf("Invalid marks! Please enter between 0 and 100.\n");
        }
      } while (s.marks < 0 || s.marks > 100);
      found = 1;
    }
    fprintf(temp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
  }
  fclose(fp);
  fclose(temp);

  if (found) {
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);
    printf("Student updated successfully.\n");
  } else {
    remove("temp.txt");
    printf("Student with Roll Number %d not found.\n", roll);
  }
}

/*
 * Deletes a student record.
 * Uses a temporary file to filter out the deleted record.
 */
void deleteStudent() {
  int roll, found = 0;
  Student s;
  FILE *fp = fopen(STUDENT_FILE, "r");
  FILE *temp = fopen("temp.txt", "w");

  if (!fp || !temp) {
    printf("Error: Unable to open file.\n");
    if (fp)
      fclose(fp);
    if (temp)
      fclose(temp);
    return;
  }

  printf("Enter Roll Number to delete: ");
  if (scanf("%d", &roll) != 1) {
    printf("Invalid input.\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
      ;
    if (fp)
      fclose(fp);
    if (temp)
      fclose(temp);
    return;
  }

  while (fscanf(fp, "%d,%49[^,],%f", &s.roll, s.name, &s.marks) == 3) {
    if (s.roll == roll) {
      found = 1;
      continue; // Skip writing this record to temp file
    }
    fprintf(temp, "%d,%s,%.2f\n", s.roll, s.name, s.marks);
  }
  fclose(fp);
  fclose(temp);

  if (found) {
    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);
    printf("Student deleted successfully.\n");
  } else {
    remove("temp.txt");
    printf("Student with Roll Number %d not found.\n", roll);
  }
}

/*
 * Changes the password of the currently logged-in user.
 */
void changePassword() {
  char newPass[50];
  char fileUser[50], filePass[50], fileRole[10];
  FILE *fp = fopen(CREDENTIALS_FILE, "r");
  FILE *temp = fopen("temp_creds.txt", "w");

  if (!fp || !temp) {
    printf("Error: Unable to open credentials file.\n");
    if (fp)
      fclose(fp);
    if (temp)
      fclose(temp);
    return;
  }

  printf("Enter new password: ");
  scanf("%49s", newPass);

  while (fscanf(fp, "%49s %49s %9s", fileUser, filePass, fileRole) == 3) {
    if (strcmp(currentUser, fileUser) == 0) {
      // Update password for current user
      fprintf(temp, "%s %s %s\n", fileUser, newPass, fileRole);
    } else {
      fprintf(temp, "%s %s %s\n", fileUser, filePass, fileRole);
    }
  }

  fclose(fp);
  fclose(temp);

  remove(CREDENTIALS_FILE);
  rename("temp_creds.txt", CREDENTIALS_FILE);
  printf("Password changed successfully.\n");
}
