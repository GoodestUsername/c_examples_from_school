#include <stdio.h>
#include <stdlib.h>

#define MAX_TEMP_STORAGE 4096
#define FILE_FAILED_TO_OPEN_ERROR 0
#define TOO_MANY_ARGS_ERROR 1
#define NOT_ENOUGH_ARGS_ERROR 2
#define ARGS_REQUIREMENT 2

void copyFileContentsToAnother(FILE *readFile, FILE *writeFile) {
    char line[MAX_TEMP_STORAGE];
    if (fgets(line, MAX_TEMP_STORAGE, readFile) == NULL) {
        return;
    }
    fprintf(writeFile, "%s", line);
    copyFileContentsToAnother(readFile, writeFile);
}

void cloneFile(char *readFileName, char* writeFileName) {
    FILE *readFile = fopen(readFileName, "r");
    FILE *writeFile = fopen(writeFileName, "w");
    if (readFile == NULL || writeFile == NULL) {
        perror("File does not exist.");
        exit(FILE_FAILED_TO_OPEN_ERROR);
    }
    copyFileContentsToAnother(readFile, writeFile);
    fclose(readFile);
    fclose(writeFile);
}

int main(int argc, char *argv[]) {
    if (argc > ARGS_REQUIREMENT) {
        perror("Too many arguments, must be two.");
        exit(TOO_MANY_ARGS_ERROR);
    }
    if (ARGS_REQUIREMENT > argc) {
        perror("Not enough arguments error, must be 2");
        exit(NOT_ENOUGH_ARGS_ERROR);
    }
    cloneFile(argv[1], argv[2]);
}

