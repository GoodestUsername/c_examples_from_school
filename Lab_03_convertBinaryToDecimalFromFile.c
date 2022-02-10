#include <stdio.h>
#include <stdlib.h>

#define CANT_OPEN_FILE 0
#define OVER_MAXIMUM_COLUMNS 1
#define MAX_COLUMNS 10
#define MAX_FILE_NAME_SIZE 1000


int fromBinary(int *binaryArray, int size) {
    if (size == 0) {
        return 0;
    }
    int number = fromBinary(binaryArray, size - 1);
    return 2 * number + binaryArray[size - 1];
}

void readNextNumberFromFile(FILE *file, int binaryArray[MAX_COLUMNS], int numberOfColumns) {
    for (int i = 0; numberOfColumns > i; i++) {
        fscanf(file, "%d", &binaryArray[i]);
    }

}

FILE *processFile(char* fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Could not open file for reading;.");
        exit(CANT_OPEN_FILE);
    }
    return file;
}

void convertBinaryToDecimalFromFile(char* fileName) {
    int numberOfRows;
    int numberOfColumns;
    int binaryArray[MAX_COLUMNS];

    FILE *file = processFile(fileName);

    fscanf(file, "%d %d", &numberOfRows, &numberOfColumns);
    
    if (numberOfColumns > MAX_COLUMNS) {
        perror("Number of columns is greater than maximum of 10.");
        exit(OVER_MAXIMUM_COLUMNS);
    }

    for (int i = 0; numberOfRows > i; i++) {
        readNextNumberFromFile(file, binaryArray, numberOfColumns);
        printf("%d\n", fromBinary(binaryArray, numberOfColumns));
    }
    fclose(file);
}

int main() {
    char fileName[MAX_FILE_NAME_SIZE];
    printf("Enter file name: ");
    scanf("%s", fileName);
    printf("\n");
    convertBinaryToDecimalFromFile(fileName);
    return 0;
}
