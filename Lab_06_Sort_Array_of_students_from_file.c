#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_OFFSET                1
#define CANT_OPEN_FILE              5
#define INITIAL_ARRAY_SIZE          0
#define MAX_NAME_SIZE               666
#define MINIMUM_GPA                 0
#define NOT_ENOUGH_ARGS_ERROR       3
#define TOO_MANY_ARGS_ERROR         4
#define COULD_NOT_ALLOCATE_MEMORY_FOR_STUDENT_NAME_ERROR 10
#define COULD_NOT_REALLOCATE_MEMORY_FOR_STUDENT_ARRAY_ERROR 11



struct Student {
    char* name;
    double gpa;
};

void freeNames(struct Student *array, int numberOfElements) {
    for (int i = 0; i <= numberOfElements; i++) {
        free(array[i].name);
    }
}

void printArrayOfStudents(struct Student *array, int numberOfElements) {
    for (int i = 0; i <= numberOfElements; i++) {
        printf("%s %f\n", array[i].name, array[i].gpa);
    }
}

FILE* processFile(char* fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open file for reading;.");
        exit(CANT_OPEN_FILE);
    }
    return file;
}

void swap(struct Student *a, struct Student *b) {
    struct Student temp = *a;
    *a = *b;
    *b = temp;
}

int partition(struct Student *array, int leftMostIndex, int rightMostIndex) {
    struct Student pivot = array[rightMostIndex];
    int storeIndex = leftMostIndex;

    for (int i = leftMostIndex; rightMostIndex > i; i++) {
        if (pivot.gpa < array[i].gpa) {
            swap(&array[storeIndex], &array[i]);
            storeIndex++;
        }
    }
    swap(&array[storeIndex], &array[rightMostIndex]);
    return storeIndex;
}

void quickSort(struct Student *array, int leftMostIndex, int rightMostIndex) {
    if (leftMostIndex > rightMostIndex) {
        return;
    }
    int pivot = partition(array, leftMostIndex, rightMostIndex);
    quickSort(array, leftMostIndex, pivot - ARRAY_OFFSET);
    quickSort(array, pivot + ARRAY_OFFSET, rightMostIndex);
}

struct Student* resizeArrayIfNeeded(struct Student *students, int sizeOfAllocation, int *usedLength, int* arrSize) {
    struct Student *arrayOfStudents;
    *usedLength = *usedLength + sizeOfAllocation;
    if (*arrSize > *usedLength) {
        return students;
    }
    *arrSize = (*arrSize + sizeOfAllocation) * 2;
    arrayOfStudents = (struct Student *)realloc(students, *arrSize);
    if (arrayOfStudents == NULL) {
        perror("Could not reallocate memory for Student Array");
        exit(COULD_NOT_REALLOCATE_MEMORY_FOR_STUDENT_ARRAY_ERROR);
    }
    students = arrayOfStudents;
    return students;
}

struct Student processStudent(char* name, double gpa, struct Student *student) {
    student = malloc(sizeof(name) + sizeof(gpa));
    if (student == NULL) {
        perror("Could not allocate memory for student name");
        exit(COULD_NOT_ALLOCATE_MEMORY_FOR_STUDENT_NAME_ERROR);
    }
    student->name = strdup(name);
    student->gpa = gpa;
    return *student;
}

struct Student* addStudentToArray(struct Student *students, struct Student *student, int processedSoFar, int *usedLength ,int *arrSize) {
    int sizeOfStudent = sizeof(*student);
    students = resizeArrayIfNeeded(students, sizeOfStudent, usedLength, arrSize);
    students[processedSoFar] = *student;
    return students;
}

struct Student* tokenizeStudentsInFile(FILE *file, struct Student *students, int *processedSoFar, int* usedLength, int* arrSize) {
    char name[MAX_NAME_SIZE];
    double gpa;
    if (fscanf(file, "%s %lf", name, &gpa) == EOF) {
        return students;
    }
    struct Student student = processStudent(name, gpa, &student);
    if (student.gpa > MINIMUM_GPA) {
        students = addStudentToArray(students, &student, *processedSoFar, usedLength, arrSize);
        (*processedSoFar)++;
    }
    return tokenizeStudentsInFile(file, students, processedSoFar, usedLength, arrSize);
}



void printStudentsInOrderOfGPA(char* fileName) {
    FILE *file = processFile(fileName);
    setbuf(stdout, NULL);
    struct Student *arrayOfStudents = malloc(INITIAL_ARRAY_SIZE);
    int arrSize = INITIAL_ARRAY_SIZE;
    int usedLength = 0;
    int studentsProcessedSoFar = 0;
    struct Student *newArrayOfStudents = tokenizeStudentsInFile(file, arrayOfStudents, &studentsProcessedSoFar, &usedLength, &arrSize);
    quickSort(newArrayOfStudents, INITIAL_ARRAY_SIZE, studentsProcessedSoFar - ARRAY_OFFSET);
    printArrayOfStudents(newArrayOfStudents, studentsProcessedSoFar - ARRAY_OFFSET);
    freeNames(newArrayOfStudents, studentsProcessedSoFar - ARRAY_OFFSET);
    free(arrayOfStudents);
    fclose(file);
}



int main(int argc, char *argv[]) {
    if (argc > 2) {
        perror("Too many arguments, must have only file name.\n");
        exit(TOO_MANY_ARGS_ERROR);
    }
    if (2 > argc) {
        perror("Not enough arguments error, must have the file name.\n");
        exit(NOT_ENOUGH_ARGS_ERROR);
    }
    printStudentsInOrderOfGPA(argv[1]);
    return 0;
}
