#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CANT_OPEN_FILE                          122332
#define TOO_MANY_ARGS_ERROR                     1
#define NOT_ENOUGH_ARGS_ERROR                   2
#define OVER_MAXIMUM_LINES_ERROR                3
#define OVER_MAXIMUM_ELEMENTS_PER_LINE_ERROR    4

#define LEFT_MOST_INDEX_OF_ARRAY                0
#define ARRAY_OFFSET                            1
#define LINE_OF_FIRST_ARRAY_IN_FILE             2
#define MAX_NUMBER_OF_ARGS_FROM_COMMAND_LINE    2
#define MAX_NUMBER_OF_LINES                     20
#define MAX_NUMBER_OF_ELEMENTS_ON_EACH_LINE     50

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *array, int leftMostIndex, int rightMostIndex) {
    int pivot = array[rightMostIndex];
    int storeIndex = leftMostIndex;

    for (int i = leftMostIndex; rightMostIndex > i; i++) {
        if (pivot > array[i]) {
            swap(&array[storeIndex], &array[i]);
            storeIndex++;
        }
    }
    swap(&array[storeIndex], &array[rightMostIndex]);
    return storeIndex;
}

void quickSort(int *array, int leftMostIndex, int rightMostIndex) {
    if (leftMostIndex > rightMostIndex) {
        return;
    }
    int pivot = partition(array, leftMostIndex, rightMostIndex);
    quickSort(array, leftMostIndex, pivot - ARRAY_OFFSET);
    quickSort(array, pivot + ARRAY_OFFSET, rightMostIndex);
}

bool checkIntArrayEquivalence(int *array1, int *array2, int arrayLength) {
    if (arrayLength == LEFT_MOST_INDEX_OF_ARRAY) {
        return true;
    }
    return (*array1 == *array2) && checkIntArrayEquivalence(array1 + ARRAY_OFFSET, array2 + ARRAY_OFFSET, arrayLength - ARRAY_OFFSET);
}

int checkAnagram(int *array1, int *array2, int arrayLength) {
    quickSort(array1,LEFT_MOST_INDEX_OF_ARRAY, arrayLength);
    quickSort(array2,LEFT_MOST_INDEX_OF_ARRAY , arrayLength);
    return checkIntArrayEquivalence(array1, array2, arrayLength);
}

void readNextArrayInFile(FILE *file, int *binaryArray, int numberOfColumns) {
    for (int i = LEFT_MOST_INDEX_OF_ARRAY; numberOfColumns > i; i++) {
        fscanf(file, "%d", &binaryArray[i]);
    }
}

void moveFilePointerToLine(FILE *file, int line) {
    char trash[MAX_NUMBER_OF_ELEMENTS_ON_EACH_LINE];

    rewind(file);
    while (line != LEFT_MOST_INDEX_OF_ARRAY) {
        fgets(trash, MAX_NUMBER_OF_ELEMENTS_ON_EACH_LINE, file);
        line--;
    }
}

bool findAnagramForCurrentLine(FILE *file, int currentLine, int *currentLineArray, int numberOfLines, int numberOfColumns) {
    int checkingLine;
    int checkingLineArray[MAX_NUMBER_OF_ELEMENTS_ON_EACH_LINE];

    for (checkingLine = currentLine; numberOfLines >= checkingLine; checkingLine++) {
        readNextArrayInFile(file, checkingLineArray, numberOfColumns);
        if (checkAnagram(currentLineArray, checkingLineArray, numberOfColumns)) {
            printf("Found lines %d and %d.\n", currentLine, checkingLine + ARRAY_OFFSET);
            return true;
        }
    }
    return false;
}

void findAnagrams(FILE *file, int numberOfLines, int numberOfColumns) {
    int line;
    int found;
    int currentLine[MAX_NUMBER_OF_ELEMENTS_ON_EACH_LINE];

    for (line = LINE_OF_FIRST_ARRAY_IN_FILE; line != numberOfLines && !found; line++) {
        readNextArrayInFile(file, currentLine, numberOfColumns);
        found = findAnagramForCurrentLine(file, line, currentLine, numberOfLines, numberOfColumns);
        moveFilePointerToLine(file, line);
    }
    if (!found) {
        printf("Not found.\n");
    }
}

FILE *processFile(char* fileName, int *numberOfLines, int *numberOfColumns) {
    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        perror("Could not open file for reading.\n");
        exit(CANT_OPEN_FILE);
    }
    fscanf(file, "%d %d", numberOfLines, numberOfColumns);
    if (*numberOfLines > MAX_NUMBER_OF_LINES) {
        perror("Number of columns is greater than maximum of 20.\n");
        exit(OVER_MAXIMUM_LINES_ERROR);
    }
    if (*numberOfColumns > MAX_NUMBER_OF_ELEMENTS_ON_EACH_LINE) {
        perror("Number of elements is greater than maximum of 50.\n");
        exit(OVER_MAXIMUM_ELEMENTS_PER_LINE_ERROR);
    }
    return file;
}

void findAnagramInFileDriver(char *fileName) {
    int numberOfLines;
    int numberOfColumns;
    FILE *file = processFile(fileName, &numberOfLines, &numberOfColumns);

    setbuf(stdout,NULL);
    findAnagrams(file, numberOfLines, numberOfColumns);
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc > MAX_NUMBER_OF_ARGS_FROM_COMMAND_LINE) {
        perror("Too many arguments, must have only file name.\n");
        exit(TOO_MANY_ARGS_ERROR);
    }
    if (LINE_OF_FIRST_ARRAY_IN_FILE > argc) {
        perror("Not enough arguments error, must have the file name.\n");
        exit(NOT_ENOUGH_ARGS_ERROR);
    }
    findAnagramInFileDriver(argv[1]);
    return 0;
}

