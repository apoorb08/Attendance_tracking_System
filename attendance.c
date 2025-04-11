#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define MAX_REG_LENGTH 20

struct Student {
    char name[MAX_NAME_LENGTH];
    char reg_no[MAX_REG_LENGTH];
    char status;
};

struct Student students[MAX_STUDENTS];
int student_count = 0;

void load_students() {
    FILE *file = fopen("students.csv", "r");
    if (file == NULL) {
        perror("\nError opening students.csv");
        exit(EXIT_FAILURE);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, ",");
        char *reg_no = strtok(NULL, ",\n");

        if (name && reg_no) {
            strncpy(students[student_count].name, name, MAX_NAME_LENGTH);
            strncpy(students[student_count].reg_no, reg_no, MAX_REG_LENGTH);
            students[student_count].status = 'A'; // Default to absent
            student_count++;

            if (student_count >= MAX_STUDENTS) {
                fprintf(stderr, "\nMaximum student capacity reached.");
                break;
            }
        }
    }
    fclose(file);
}

void load_attendance() {
    FILE *file = fopen("attendance.csv", "r");
    if (file == NULL) return;

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char *reg_no = strtok(line, ",");
        char *status = strtok(NULL, ",\n");

        if (reg_no && status) {
            for (int i = 0; i < student_count; i++) {
                if (strcmp(students[i].reg_no, reg_no) == 0) {
                    students[i].status = toupper(status[0]);
                    break;
                }
            }
        }
    }
    fclose(file);
}

void save_attendance() {
    FILE *file = fopen("attendance.csv", "w");
    if (file == NULL) {
        perror("\nError saving attendance data");
        return;
    }

    for (int i = 0; i < student_count; i++) {
        fprintf(file, "%s,%c\n", students[i].reg_no, students[i].status);
    }
    fclose(file);
}

void show_students() {
    printf("\n%-20s %-15s\n", "Name", "Registration No");
    printf("----------------------------------------\n");
    for (int i = 0; i < student_count; i++) {
        printf("%-20s %-15s\n", students[i].name, students[i].reg_no);
    }
}

void mark_attendance() {
    char reg_no[MAX_REG_LENGTH];
    printf("\nEnter registration number: ");
    scanf("%s", reg_no);

    int found = -1;
    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].reg_no, reg_no) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Student not found!\n");
        return;
    }

    char status;
    printf("Enter status (P/A): ");
    scanf(" %c", &status);
    status = toupper(status);

    if (status != 'P' && status != 'A') {
        printf("Invalid status! Use P or A.\n");
        return;
    }

    students[found].status = status;
    printf("Attendance updated for %s.\n", students[found].name);
}

void show_summary() {
    int present = 0, absent = 0;
    for (int i = 0; i < student_count; i++) {
        students[i].status == 'P' ? present++ : absent++;
    }
    printf("\nAttendance Summary:\n");
    printf("Present: %d\nAbsent: %d\n", present, absent);
}

void show_student_attendance() {
    char reg_no[MAX_REG_LENGTH];
    printf("\nEnter registration number: ");
    scanf("%s", reg_no);

    for (int i = 0; i < student_count; i++) {
        if (strcmp(students[i].reg_no, reg_no) == 0) {
            printf("\nStudent Details:\n");
            printf("Name: %s\nRegistration No: %s\nStatus: %s\n",
                   students[i].name,
                   students[i].reg_no,
                   students[i].status == 'P' ? "Present" : "Absent");
            return;
        }
    }
    printf("Student not found!\n");
}

int main() {
    load_students();
    load_attendance();

    int choice;
    do {
        printf("\nAttendance Tracking System\n");
        printf("1. Show Students List\n");
        printf("2. Mark Attendance\n");
        printf("3. Show Summary\n");
        printf("4. Show Student Attendance\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: show_students(); break;
            case 2: mark_attendance(); break;
            case 3: show_summary(); break;
            case 4: show_student_attendance(); break;
            case 5: save_attendance(); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 5);

    printf("\nExiting system...\n");
    return 0;
}